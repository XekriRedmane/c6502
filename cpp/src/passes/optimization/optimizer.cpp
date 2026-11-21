/* The TAC optimizer's driver.

It wraps SSA construction, a fixed point, and SSA destruction around a
function. The fixed point sweeps its passes again until a sweep leaves
the function's instructions equal to what they were at the start of
the sweep. Each sweep runs every pass, whether or not the earlier ones
changed anything, since a pass at its fixed point is cheap to run again
and the interleaving of the passes is part of the optimizer's contract.

The shape of the pipeline (which `PhaseDriver` runs) is

    fn → loop_rotate (once, before SSA)
       → SSA construction
       → fold_static_const_reads (once, before the fixed point)
       → (CF → strength_reduce → cmp_zero_jump_fold → and_zero_jump_fold →
          lnot_jump_fold → dead_loop_elim → thread_constant_jumps →
          UCE → CopyProp → DSE →
          CopyFold → ...)*
       → recognize_indirect_indexed (once, after the fixed point)
       → hoist_subtract_to_compare, hoist_divmod_pairs,
         count_loops_down (once each, after the fixed point)
       → SSA destruction
       → CopyFold (after destruction)
       → move_steps_onto_edges (after destruction)
       → split_three_way_compares (after destruction)
       → fold_short_circuit_jump* (after destruction, until it converges)
       → fn'

`loop_rotate` runs before SSA because its rewrite moves ranges of
instructions and renames nothing. Before SSA the loop counter `x_var`
has one name across the init, the body, and the post block, so moving
the instructions is enough. After the rotation, `to_ssa` builds the
Phis for the new control flow.

The promotable Vars (block-scope locals, parameters, and TAC
temporaries whose address is never taken and whose type is scalar) are
renamed and joined by Phis between SSA construction and destruction.
Address-taken locals, statics, and aggregates pass through unchanged.

The roles of the passes:
  - constant_fold folds a Unary, a Binary, or a cast of Constant
    operands, and a Phi whose sources all agree into a Copy.
  - strength_reduce rewrites a Multiply, an unsigned Divide, or an
    unsigned Modulo by a power of 2 as a LeftShift, a RightShift, or a
    BitwiseAnd.
  - cmp_zero_jump_fold rewrites `Binary(==/!=, x, 0, cond);
    JumpIfTrue/False(cond, t)` as a JumpIf on x itself, with the sense
    flipped where needed, following ZeroExtend definitions to work at
    the narrowest width available.
  - eliminate_dead_loops finds natural loops whose body is pure (no
    Call, Store, or Ret) and whose SSA definitions are all read only
    inside the loop, and makes the header jump past the body, so that
    UCE prunes it in the next sweep. With DSE it removes nested empty
    loops entirely.
  - UCE prunes unreachable blocks, folds a Phi with one argument to a
    Copy, and counts a Phi's predecessor labels as uses of the labels,
    so that SSA destruction can find the predecessors later.
  - copy_propagate and eliminate_dead_stores are the versions for SSA
    form.
  - fold_copies fuses a pair `<producer dst=%t>; Copy(%t, X)` into
    `<producer dst=X>` when `%t` is read once. That removes the trip
    through the temporary when the Copy's dst isn't an SSA name, which
    copy propagation and DSE can't reach, typically the read, change,
    and write of a static, as in `static int x; x++;`.

Termination. Each pass is a pure function of the `tac_ast.Function`,
and `==` compares structurally, so the loop ends as soon as no pass of
a sweep changed anything.

The program. Only the `Function`s of a program are optimized. The
`StaticVariable`s pass through unchanged, since their `init` is a
constant layout of bytes and not control flow.

Without a symbol table, as some older unit tests of the driver on
synthetic functions call it, `optimize_function` skips SSA
construction, since the table is needed to give the new SSA names
their types. Skipping is better than making temporaries without types
that later passes can't size.

The driver doesn't allocate registers. That happens in the asm
optimizer (src/passes/optimization_asm/), on the IR after `tac_to_asm`,
byte by byte.

The table. The port of each pass lands with its group (docs/cpp_port.md,
"Step 5"), and its entry below holds null until then. The driver throws
`InternalError` when it reaches an entry without a port, so a compile
runs through C++ only once the table is full (`PhaseDriver::complete`).
*/
#include "passes/optimization/optimizer.h"

#include <utility>

#include "passes/optimization/constant_folding.h"
#include "passes/optimization/distribute_constant.h"
#include "passes/optimization/loop_rotate.h"
#include "passes/optimization/noreturn_call.h"
#include "passes/optimization/phi_constant_fold.h"
#include "passes/optimization/static_const_fold.h"
#include "passes/optimization/truncate_extend_fold.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::NamedPass;
using framework::PhaseDriver;

PhaseDriver make_driver() {
  PhaseDriver d;
  d.pre_ssa = {
      // Rotates the signed count-down for loops to test at the bottom.
      // It works on c99_to_tac's layout of a for loop, where x_var has
      // one name across the init, the body, and the post block, and
      // `to_ssa` builds the Phis for the rotated control flow after it.
      {"rotate_signed_countdown_loops", &rotate_signed_countdown_loops},
      // Nothing runs after a call to a `[[noreturn]]` function.
      {"noreturn_calls", &noreturn_calls},
  };
  d.pre_fixedpoint = {
      // Once: replaces each read of `Var(static_const_scalar)` with
      // `Constant(value)`, so that the fixed point's constant folding
      // can fold the arithmetic after it. SSA construction is done by
      // then, so the substitution doesn't disturb the chains of
      // definitions and uses (statics aren't promoted anyway).
      {"fold_static_const_reads", &fold_static_const_reads},
  };
  d.fixedpoint = {
      {"constant_fold", &constant_fold},
      // `(x - c) * k` on a widened byte `x` as `x * k - c * k`, before
      // strength reduction makes the multiply shifts.
      {"distribute_constant", &distribute_constant},
      // An operation of a Phi of constants is computed in the Phi's
      // arguments, so `(c ? 3 : 2) + 1` joins 4 and 3.
      {"fold_into_constant_phi", &fold_into_constant_phis},
      // Strength reduction, as one RuleSet of rules for each operation
      // (a Multiply, Divide, or Modulo by a power of two).
      {"reduce_strength", nullptr},
      // Three folds of a jump on the result of a comparison, a
      // BitwiseAnd, or a LogicalNot read once by a JumpIf, as one
      // RuleSet. They share the window of the producer and the JumpIf
      // and the test that the result is read once, and their producers
      // are different operations, so one sweep applies whichever rule
      // matches each position in place of three sweeps. A compare kept
      // as a value narrows its operands when the comparison's fold,
      // tried first at the same position, finds no jump on its result.
      // A JumpIfCmp made before its operand's extend was exposed
      // (through a cast that copy propagation removed later) narrows
      // the same way. A `!` of a value that is 0 or 1 and that no jump
      // tests is `^ 1`. A signed ordering compare of two values that
      // can't be negative is done unsigned, last, once nothing narrows
      // it.
      {"fold_producer_jumps", nullptr},
      {"eliminate_dead_loops", nullptr},
      // An edge that brings a join a constant its test decides goes
      // where the test sends it, so an inlined `return false;` doesn't
      // load 0 and branch on it. Before UCE, which drops the join.
      {"thread_constant_jumps", nullptr},
      {"eliminate_unreachable_code", nullptr},
      // Replaces a repeated computation or load with a Copy from the
      // first one, which copy_propagate then forwards.
      {"eliminate_common_subexpressions", nullptr},
      {"hoist_loop_invariants", nullptr},
      {"copy_propagate", nullptr},
      {"eliminate_dead_stores", nullptr},
      // `*++P` on a global P reads P after the copy into it, so the copy
      // folding next makes the add write P directly.
      {"read_stored_globals", nullptr},
      {"fold_copies_in_fixedpoint", nullptr},
      {"reassoc_constants", nullptr},
      {"reassoc_offset_constant", nullptr},
      // `g.m` and `r.m` as direct byte accesses in place of a store
      // through `&g + k`. After the reassociations, so the offset is one
      // Constant.
      {"direct_member_access", nullptr},
      {"recognize_indexed_store", nullptr},
      {"recognize_indexed_load", nullptr},
      {"fold_truncate_extend", &fold_truncate_extend},
      {"fold_extend_extend", &fold_extend_extend},
      {"sink_increments", nullptr},
      {"sink_and_past_branch", nullptr},
      // After sink_and_past_branch, so that the latter sees the wide
      // `ZeroExtend + BitwiseAnd + Truncate + JumpIfMasked` trio it
      // matches. The narrow form this pass makes (Binary(BitwiseAnd,
      // %x, ConstUChar(C), %t)) wouldn't match the sinking pass's strict
      // shape, which would leave the AND's result live across the
      // branch and spill it to memory.
      {"narrow_widened_arith", nullptr},
      // `pos / 20`, `(style >> 5)`, and `price * (tier + 1)` on bytes
      // promoted to `int` as byte operations, when value_range bounds
      // the operands by 255.
      {"narrow_byte_range", nullptr},
      // `(x & 0x3F) < 3` on a byte x as a byte `&`, zero-extended, so
      // the comparison narrows. After sink_and_past_branch for the same
      // reason as narrow_widened_arith.
      {"narrow_byte_bitwise", nullptr},
  };
  d.post_fixedpoint = {
      // A join choosing `p + k1` or `p + k2` as a choice of `k1` or `k2`
      // added to `p` after it, which the next two passes make an
      // indexed access.
      {"phi_offset", nullptr},
      // `o[i].m` on a static array as `IndexedLoad(o, i, offset=m)`
      // (abs,X), and `(&o + i) + m` as `(&o + m) + i`. Before the
      // indirect-indexed recognizer, which would otherwise take the
      // access as `(zp),Y` through a pointer.
      {"static_indexed_access", nullptr},
      // The indirect-indexed recognizer runs after the fixed point has
      // converged on constant folding. Inside the loop it could lock in
      // an IndirectIndexed form for an address chain whose pointer is
      // about to fold to a Constant in the next sweep, ahead of the
      // cheaper `recognize_indexed_store` lowering. Running it last
      // means that every chain that could become absolute,X has
      // (through recognize_indexed_store), and only the cases with a
      // pointer at run time (zp_abi parameters, pointer locals whose
      // address is taken) are left for the (zp),Y lowering.
      {"recognize_indirect_indexed", nullptr},
      // `t = a - b` next to the `a < b` (or `>=`, ...) that decides
      // whether it runs, so tac_to_asm branches on its flags in place
      // of comparing too. After the fixed point, whose sinking passes
      // would move it back.
      {"hoist_subtract_to_compare", nullptr},
      // A `/` and a `%` of the same operands with a store, a call, or a
      // branch between them computed next to each other, so that
      // tac_to_asm lowers them with one loop or one helper call.
      {"hoist_divmod_pairs", nullptr},
      // A loop counter read only by its own test and step counts the
      // passes left down to 0, so its test is `DEC i; BNE`.
      {"count_loops_down", nullptr},
  };
  d.post_destruction = {
      // Copy folding after from_ssa. SSA destruction emits a Copy at
      // the end of each predecessor that feeds each Phi's source into
      // the Phi's dst. For a loop counter's `i++` that gives
      // `Binary(Add, i.vK, 1, %t); Copy(%t, i.vJ)` at the end of the
      // loop's continue block, which the fold makes the in-place
      // `Binary(Add, i.vK, 1, i.vJ)`. Once after destruction is enough,
      // without running the whole fixed point again, because nothing
      // later in the TAC pipeline makes new pairs to fuse.
      {"fold_copies_post_destruction", nullptr},
      // `do ...; while (i-- != first);` computes `i - 1` before the test
      // and copies it into `i` on the back edge alone. The step goes in
      // place of that copy, where it's `DEC i`.
      {"move_steps_onto_edges", nullptr},
      // `a < b` and `a == b` on the same 2-byte values as a compare of
      // the high bytes and then the low ones. After destruction, since
      // the new edges would need Phi arguments.
      {"split_three_way_compares", nullptr},
  };
  d.post_destruction_fixedpoint = {
      // The short-circuit fold after destruction: the tail of `&&` or
      // `||` that makes 0 or 1, and the JumpIf next to it that reads
      // it, become conditional branches. Before destruction the tail is
      // split across two SSA definitions of `%t` joined by a Phi. After
      // destruction (and the copy folding above) it's the canonical
      // tail of five instructions this pass matches. The fixed point
      // peels nested shapes (`(a && b) || c` and the like) one
      // short-circuit at a time.
      {"fold_short_circuit_jump", nullptr},
      // A jump whose target tests what the jump's own path decided, as
      // the second test of `x == a || x == b` reaching a test of
      // `x == a` in the `if`, goes past the test.
      {"thread_known_compares", nullptr},
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
