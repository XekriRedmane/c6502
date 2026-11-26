#include "passes/optimization/optimizer.h"

#include <utility>

#include "passes/optimization/compare_subtract.h"
#include "passes/optimization/constant_folding.h"
#include "passes/optimization/copy_folding.h"
#include "passes/optimization/copy_propagation.h"
#include "passes/optimization/count_down.h"
#include "passes/optimization/cse.h"
#include "passes/optimization/dead_loop_elimination.h"
#include "passes/optimization/dead_store_elimination.h"
#include "passes/optimization/direct_member_access.h"
#include "passes/optimization/distribute_constant.h"
#include "passes/optimization/divmod_hoist.h"
#include "passes/optimization/fold_producer_jumps.h"
#include "passes/optimization/jump_thread.h"
#include "passes/optimization/known_compare_thread.h"
#include "passes/optimization/licm.h"
#include "passes/optimization/loop_rotate.h"
#include "passes/optimization/narrow_byte_bitwise.h"
#include "passes/optimization/narrow_byte_range.h"
#include "passes/optimization/narrow_widened_arith.h"
#include "passes/optimization/noreturn_call.h"
#include "passes/optimization/phi_constant_fold.h"
#include "passes/optimization/phi_offset.h"
#include "passes/optimization/read_stored_global.h"
#include "passes/optimization/reassoc_const.h"
#include "passes/optimization/reassoc_offset.h"
#include "passes/optimization/recognize_indexed_load.h"
#include "passes/optimization/recognize_indexed_store.h"
#include "passes/optimization/recognize_indirect_indexed.h"
#include "passes/optimization/short_circuit_jump_fold.h"
#include "passes/optimization/sink_and_past_branch.h"
#include "passes/optimization/sink_increment.h"
#include "passes/optimization/static_const_fold.h"
#include "passes/optimization/static_indexed_access.h"
#include "passes/optimization/step_on_edge.h"
#include "passes/optimization/strength_reduction.h"
#include "passes/optimization/three_way_compare.h"
#include "passes/optimization/truncate_extend_fold.h"
#include "passes/optimization/unreachable_code_elimination.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::NamedPass;
using framework::PhaseDriver;

PhaseDriver make_driver() {
  PhaseDriver d;
  d.pre_ssa = {
      {"rotate_signed_countdown_loops", &rotate_signed_countdown_loops,
       "RotateSignedCountdownLoops"},
      {"noreturn_calls", &noreturn_calls, "NoreturnCalls"},
  };
  d.pre_fixedpoint = {
      {"fold_static_const_reads", &fold_static_const_reads, "FoldStaticConstReads"},
  };
  d.fixedpoint = {
      {"constant_fold", &constant_fold, "ConstantFold"},
      {"distribute_constant", &distribute_constant, "DistributeConstant"},
      {"fold_into_constant_phi", &fold_into_constant_phis, "FoldIntoConstantPhi"},
      {"reduce_strength", &reduce_strength, "RuleSet"},
      {"fold_producer_jumps", &fold_producer_jumps, "RuleSet"},
      {"eliminate_dead_loops", &eliminate_dead_loops, "EliminateDeadLoops"},
      {"thread_constant_jumps", &thread_constant_jumps, "ThreadConstantJumps"},
      {"eliminate_unreachable_code", &eliminate_unreachable_code, "EliminateUnreachableCode"},
      {"eliminate_common_subexpressions", &eliminate_common_subexpressions,
       "EliminateCommonSubexpressions"},
      {"hoist_loop_invariants", &hoist_loop_invariants, "HoistLoopInvariants"},
      {"copy_propagate", &copy_propagate, "CopyPropagate"},
      {"eliminate_dead_stores", &eliminate_dead_stores, "EliminateDeadStores"},
      {"read_stored_globals", &read_stored_globals, "ReadStoredGlobals"},
      {"fold_copies_in_fixedpoint", &fold_copies_in_fixedpoint, "FoldCopiesInFixedpoint"},
      {"reassoc_constants", &reassoc_constants, "ReassocConstants"},
      {"reassoc_offset_constant", &reassoc_offset_constant, "ReassocOffsetConstant"},
      {"direct_member_access", &direct_member_access, "DirectMemberAccess"},
      {"recognize_indexed_store", &recognize_indexed_store, "RecognizeIndexedStore"},
      {"recognize_indexed_load", &recognize_indexed_load, "RecognizeIndexedLoad"},
      {"fold_truncate_extend", &fold_truncate_extend, "FoldTruncateExtend"},
      {"fold_extend_extend", &fold_extend_extend, "FoldExtendExtend"},
      {"sink_increments", &sink_increments, "SinkIncrements"},
      {"sink_and_past_branch", &sink_and_past_branch, "SinkAndPastBranch"},
      {"narrow_widened_arith", &apply_narrow_widened_arith, "NarrowWidenedArith"},
      {"narrow_byte_range", &narrow_byte_range, "NarrowByteRange"},
      {"narrow_byte_bitwise", &narrow_byte_bitwise, "NarrowByteBitwise"},
  };
  d.post_fixedpoint = {
      {"phi_offset", &phi_offset, "PhiOffset"},
      {"static_indexed_access", &static_indexed_access, "StaticIndexedAccess"},
      {"recognize_indirect_indexed", &recognize_indirect_indexed, "RecognizeIndirectIndexed"},
      {"hoist_subtract_to_compare", &hoist_subtract_to_compare, "HoistSubtractToCompare"},
      {"hoist_divmod_pairs", &hoist_divmod_pairs, "HoistDivmodPairs"},
      {"count_loops_down", &count_loops_down, "CountLoopsDown"},
  };
  d.post_destruction = {
      {"fold_copies_post_destruction", &fold_copies_post_destruction, "FoldCopiesPostDestruction"},
      {"move_steps_onto_edges", &move_steps_onto_edges, "MoveStepsOntoEdges"},
      {"split_three_way_compares", &split_three_way_compares, "SplitThreeWayCompares"},
  };
  d.post_destruction_fixedpoint = {
      {"fold_short_circuit_jump", &fold_short_circuit_jumps, "FoldShortCircuitJump"},
      {"thread_known_compares", &thread_known_compares, "ThreadKnownCompares"},
  };
  return d;
}

}  // namespace

const framework::PhaseDriver& tac_driver() {
  static const PhaseDriver driver = make_driver();
  return driver;
}

ir::Function optimize_function(ir::Function fn, SymbolTable* symbols) {
  framework::PassContext ctx{.symbols = symbols};
  NameSet ssa;
  return tac_driver().apply(std::move(fn), ctx, ssa);
}

ir::Program optimize_program(ir::Program prog, SymbolTable* symbols) {
  for (auto& t : prog.top_level) {
    if (auto* fn = asdl::as<ir::Function>(t)) {
      *fn = optimize_function(std::move(*fn), symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
