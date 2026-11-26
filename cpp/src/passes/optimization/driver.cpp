// The port of passes/optimization/framework/driver.py. The driver runs
// an ordered pipeline of passes in phases over one function. Python's
// `PhaseDriver` checks at construction that each list holds passes of
// its phase's class. The C++ lists hold the passes' functions, and the
// table that builds them (src/passes/optimization/optimizer.cpp) puts
// each in its Python list.
#include "passes/optimization/driver.h"

#include <string>
#include <utility>

#include "errors.h"
#include "pass_order.h"
#include "passes/ir_check.h"
#include "passes/optimization/ssa_construction.h"
#include "passes/optimization/ssa_destruction.h"

namespace c6502::passes::framework {

namespace {

namespace ir = ::c6502::tac_ast;

const Observer* current_observer = nullptr;

void observe(const ir::Function& fn, std::string_view after) {
  if (current_observer != nullptr) (*current_observer)(fn, after);
}

// Runs `p` on `fn` and, with the IR checks on, checks the result, with
// the SSA names when `in_ssa` is true, and then calls the observer, in
// Python's order.
ir::Function run(const NamedPass& p, ir::Function fn, const PassContext& ctx, bool in_ssa) {
  if (p.run == nullptr) {
    throw InternalError("the TAC pass " + std::string(p.name) + " isn't ported to C++");
  }
  fn = p.run(std::move(fn), ctx);
  ir_check::check_tac(fn, p.class_name, ctx.symbols, in_ssa ? ctx.ssa_dsts : nullptr);
  observe(fn, p.class_name);
  return fn;
}

// Sweeps `passes` over `fn` until a sweep leaves it equal to what the
// sweep started with, or `cap` times. Python compares with `==`, so a
// pass that rebuilds an equal function ends the fixed point too.
ir::Function sweep(const std::vector<NamedPass>& passes, ir::Function fn, const PassContext& ctx,
                   int cap, bool in_ssa) {
  for (int i = 0; i < cap; ++i) {
    const ir::Function prev = fn;
    for (const auto& p : passes) {
      fn = run(p, std::move(fn), ctx, in_ssa);
    }
    if (fn == prev) {
      break;
    }
  }
  return fn;
}

// Reports the passes of one of the driver's lists, which run next
// (src/pass_order.h).
void report(const std::vector<NamedPass>& passes) {
  for (const auto& p : passes) {
    pass_order::ran(p.name);
  }
}

bool all_ported(const std::vector<NamedPass>& passes) {
  for (const auto& p : passes) {
    if (p.run == nullptr) {
      return false;
    }
  }
  return true;
}

}  // namespace

const Observer* set_observer(const Observer* observer) {
  const Observer* previous = current_observer;
  current_observer = observer;
  return previous;
}

bool PhaseDriver::complete() const {
  return all_ported(pre_ssa) && all_ported(pre_fixedpoint) && all_ported(fixedpoint) &&
         all_ported(post_fixedpoint) && all_ported(post_destruction) &&
         all_ported(post_destruction_fixedpoint);
}

ir::Function PhaseDriver::apply(ir::Function fn, PassContext& ctx, NameSet& ssa_storage) const {
  const pass_order::Run pass_run("tac");
  ir_check::check_tac(fn, "c99_to_tac", ctx.symbols);
  // The passes before SSA run whether or not SSA construction will.
  report(pre_ssa);
  for (const auto& p : pre_ssa) {
    fn = run(p, std::move(fn), ctx, false);
  }
  if (ctx.symbols != nullptr) {
    auto [ssa_fn, ssa_dsts] = to_ssa(std::move(fn), *ctx.symbols);
    fn = std::move(ssa_fn);
    ssa_storage = std::move(ssa_dsts);
    ctx.ssa_dsts = &ssa_storage;
    observe(fn, "to_ssa");
    ir_check::check_tac(fn, "to_ssa", ctx.symbols, ctx.ssa_dsts);
    report(pre_fixedpoint);
    for (const auto& p : pre_fixedpoint) {
      fn = run(p, std::move(fn), ctx, true);
    }
  }
  // The main fixed point.
  report(fixedpoint);
  fn = sweep(fixedpoint, std::move(fn), ctx, fixedpoint_cap, ctx.symbols != nullptr);
  if (ctx.symbols != nullptr) {
    report(post_fixedpoint);
    for (const auto& p : post_fixedpoint) {
      fn = run(p, std::move(fn), ctx, true);
    }
    fn = from_ssa(std::move(fn), ctx.symbols);
    observe(fn, "from_ssa");
    ir_check::check_tac(fn, "from_ssa", ctx.symbols);
  }
  // The passes after destruction, once each.
  report(post_destruction);
  for (const auto& p : post_destruction) {
    fn = run(p, std::move(fn), ctx, false);
  }
  // The fixed point after destruction.
  report(post_destruction_fixedpoint);
  return sweep(post_destruction_fixedpoint, std::move(fn), ctx, fixedpoint_cap, false);
}

}  // namespace c6502::passes::framework
