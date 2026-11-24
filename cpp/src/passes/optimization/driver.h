// The port of passes/optimization/framework/driver.py, the
// `PhaseDriver` that runs the TAC optimizer's passes on one function in
// six lists, with SSA construction and destruction between them and two
// fixed points.
#pragma once

#include <functional>
#include <string_view>
#include <vector>

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes::framework {

// A pass of the TAC optimizer, as the C++ port runs it: the function
// and the context, whose symbol table and SSA names it may add to.
using TacPass = tac_ast::Function (*)(tac_ast::Function fn, const PassContext& ctx);

// One entry of a driver's list: the pass's `name`, as Python's class
// gives it and tools/capture_passes.py records it (`tac_<name>`), and
// its port, which is null until the pass is ported.
struct NamedPass {
  std::string_view name;
  TacPass run = nullptr;
};

// Called as `observer(fn, after)` with the function after every pass,
// with the pass's `name`, and after SSA construction and destruction,
// with "to_ssa" and "from_ssa". It stands for Python's class variable
// `PhaseDriver.observer`, which fuzz/tac_passes.py sets to run the
// program after each step (src/tac_sim_modes.h).
using Observer = std::function<void(const tac_ast::Function& fn, std::string_view after)>;

// Makes every driver call `observer`, or none when it is null, and
// returns the observer it replaces.
const Observer* set_observer(const Observer* observer);

// The driver's lists, which Python types by their phase classes
// (`PreSsaPass`, `FixedpointPass`, ...). C++ checks the phases only by
// where a table puts each pass.
struct PhaseDriver {
  std::vector<NamedPass> pre_ssa;
  std::vector<NamedPass> pre_fixedpoint;
  std::vector<NamedPass> fixedpoint;
  std::vector<NamedPass> post_fixedpoint;
  std::vector<NamedPass> post_destruction;
  std::vector<NamedPass> post_destruction_fixedpoint;

  // The most sweeps either fixed point makes. A fixed point that hasn't
  // converged by then stops with the function as the last sweep left
  // it, without an error, as Python's does.
  int fixedpoint_cap = 256;

  // Returns whether every pass of the six lists has a port.
  bool complete() const;

  // Runs the lists on `fn` and returns the result. The `pre_ssa` passes
  // run first, once each. With a symbol table, SSA construction follows
  // and sets `ctx.ssa_dsts` to the names it made, which `ssa_storage`
  // holds, and then the `pre_fixedpoint` passes run once each. The
  // `fixedpoint` passes are swept until a sweep leaves the function
  // equal to what it started with, or `fixedpoint_cap` times. With a
  // symbol table, the `post_fixedpoint` passes then run once each, and
  // SSA destruction follows. `ctx.ssa_dsts` stays set after it, as
  // Python's does, so the passes after destruction see the SSA names
  // too. Last come the `post_destruction` passes once each and the
  // `post_destruction_fixedpoint` passes, swept as the first fixed
  // point is. Without a symbol table, SSA construction, the passes in
  // SSA form, and destruction are skipped, which only the unit tests of
  // synthetic functions do.
  //
  // Python checks the TAC after every pass under `C6502_CHECK_IR=1`,
  // which the C++ driver doesn't. Both call the observer that
  // `set_observer` gives. Throws `InternalError` for a pass that has no
  // port.
  tac_ast::Function apply(tac_ast::Function fn, PassContext& ctx, NameSet& ssa_storage) const;
};

}  // namespace c6502::passes::framework
