// The asm back end after prologue synthesis, ported from compile.py.
//
// Under `-O`, `optimized_asm` runs the passes below on the program that
// `synthesize_prologue` returns, from `apply_licm` through
// `apply_outline_runs`, and then places the zero-page globals. Without
// `-O`, `_codegen` runs the peephole fixed point and the expansion of
// long branches on the program `replace_pseudoregisters` returns. Each
// pass here is the port of the Python pass of the same name, and each
// is given the arguments Python gives it. The comments on the steps
// are compile.py's.
//
// compile.py hands the region to this code under `C6502_CPP_BACKEND=1`
// (cpp/README.md, "The back end through c6502cpp"), and enters the
// functions that `apply_shared_tails` and `apply_outline_runs` make in
// its symbol table, as the Python passes do.
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
  // Python's `{**all_slot_symbols, **zp.addrs}`, the map LICM and the
  // counter promotions read the zero-page bytes from, in which a global's
  // address takes the place of a slot's of the same name.
  SymbolAddrs pointer_symbols = args.slot_symbols;
  for (const auto& [name, addr] : args.zp_addrs) {
    pointer_symbols[name] = addr;
  }
  // The five fixed points have the same keyword arguments, so they share
  // one sweep.
  const std::vector<SweepPass> sweep =
      peephole_sweep(ctx, &args.slot_symbols, &args.zp_addrs, args.noreturn);
  RegionResult out;
  const pass_order::Run pass_run("optimized_asm");
  // LICM-lite: hoist loop-invariant constant stores out of natural
  // loops. It fires only when the loop body has no call, which sidesteps
  // the question of what a zp_abi callee overwrites.
  pass_order::ran("licm");
  prog = apply_licm(std::move(prog), ctx, &pointer_symbols);
  prog = run_to_fixed_point(std::move(prog), sweep);
  // Promote a reg('A') parameter's body-local spill to live in A across
  // the loop body (it parks the value in Y during the preamble's staging,
  // which overwrites A). It runs alongside the X-pivot promotion below,
  // since both target a memory cell whose lifetime is a single loop.
  pass_order::ran("promote_loop_carried_to_a");
  prog = apply_promote_loop_carried_to_a(std::move(prog));
  pass_order::ran("loop_counter_to_x");
  prog = apply_loop_counter_to_x(std::move(prog), &pointer_symbols);
  // A count-down counter used only by its DEC goes in X (or Y) when the
  // loop leaves that register alone.
  pass_order::ran("counter_to_index_reg");
  prog = apply_counter_to_index_reg(std::move(prog), &pointer_symbols);
  // Catch the cases the X-pivot promotion couldn't reach: a pseudo that
  // the register allocator put in X but that kept an `LDA M` read of its
  // spill home M for passing an argument, which reads a stale M after
  // DEX. Those `LDA M` become `TXA`. This runs before the fixed point
  // after the promotion, so that the `STX M` / `LDX M` wraps it leaves
  // dead are cleaned up there.
  pass_order::ran("x_save_slot_load");
  prog = apply_x_save_slot_load(std::move(prog));
  // The second fixed point catches what the promotion exposed, such as
  // redundant `LDX` / `STX` pairs once the Y-pivot inside
  // loop_counter_to_x has freed X.
  prog = run_to_fixed_point(std::move(prog), sweep);
  // Promote to Y a `Data` symbol that's loaded into X several times when
  // Y is otherwise unused, which saves each `LDX abs` reload by loading
  // it into Y once at the function's entry. The fixed point runs again
  // so that the dead `LDA` / `STA` pairs around the dropped `LDX` go.
  pass_order::ran("dual_index_promotion");
  prog = apply_dual_index_promotion(std::move(prog));
  prog = run_to_fixed_point(std::move(prog), sweep);
  // Drop a SEC or CLC where C already has that value, or move one in
  // front of a loop whose back edges have it (carry_known). This runs
  // after the peepholes, several of which match a SEC or CLC.
  pass_order::ran("carry_known");
  prog = apply_carry_known(std::move(prog));
  // A local holding a copy of a parameter gets the parameter's own slot
  // when the two are never live with different values
  // (slot_coalescing). It runs after the promotions and peepholes that
  // look for `__local_*` slots, and before the fixed points after the
  // loop rotation, which drop the copies it leaves as `LDA P; STA P`.
  pass_order::ran("slot_coalescing");
  auto [coalesced, written] =
      apply_slot_coalescing(std::move(prog), args.abi, args.shares, args.slot_symbols);
  prog = std::move(coalesced);
  out.coalesced_params = std::move(written);
  // A local pair holding a copy of a static function's zero-page result,
  // `F@ret`, becomes `F@ret` itself when the two are never live with
  // different values (ret_slot_coalescing).
  pass_order::ran("ret_slot_coalescing");
  prog = apply_ret_slot_coalescing(std::move(prog), args.slot_symbols, args.zp_addrs,
                                   args.ret_eligible);
  // Keep a loop-invariant compare key in A across its loop
  // (loop_invariant_a). It runs after the peepholes have settled the
  // layout it inspects.
  pass_order::ran("loop_invariant_a");
  prog = apply_loop_invariant_a(std::move(prog), ctx, &args.slot_symbols);
  // Move each loop's test to its bottom (rotate_loop_test). This runs
  // after loop_invariant_a, which looks for the test at the top.
  pass_order::ran("rotate_loop_test");
  prog = apply_rotate_loop_test(std::move(prog), ctx);
  // The rotation leaves shapes that the peepholes and the counter
  // promotion look for, such as `DEC i; LDA i; BNE` at a loop's new
  // bottom, so both run again, and carry_known after them.
  prog = run_to_fixed_point(std::move(prog), sweep);
  pass_order::ran("counter_to_index_reg");
  prog = apply_counter_to_index_reg(std::move(prog), &pointer_symbols);
  prog = run_to_fixed_point(std::move(prog), sweep);
  pass_order::ran("carry_known");
  prog = apply_carry_known(std::move(prog));
  // A chain of compares of A with constants becomes a jump table
  // (jump_table), after every pass that follows control flow.
  pass_order::ran("jump_tables");
  prog = apply_jump_tables(std::move(prog));
  // A byte copied while A is busy goes through X or Y
  // (copy_through_index), once the peepholes that fold the loads around
  // such a copy have run, and after the jump tables, whose bounds check
  // makes N and Z dead after a switch's load.
  pass_order::ran("copy_through_index");
  prog = apply_copy_through_index(std::move(prog), ctx, &args.slot_symbols);
  pass_order::ran("long_branches");
  prog = expand_long_branches(std::move(prog));
  // Jumps to tail-call stubs and jumps after a constant load, with the
  // branches' final reach (short_jumps).
  pass_order::ran("short_jumps");
  prog = apply_short_jumps(std::move(prog));
  // Code that two functions end with alike goes in a function of its
  // own, which each jumps to (shared_tails). Neither this pass nor the
  // next moves a `(zp),Y` pointer in a slot's bytes into another
  // function, where `asm_emit` would name it by that function's slot.
  pass_order::ran("shared_tails");
  prog = apply_shared_tails(std::move(prog), &args.zp_names, args.slot_addrs,
                            &out.new_functions);
  // A run of instructions repeated in the unit goes in a function of its
  // own, which each place calls (outline_runs).
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
