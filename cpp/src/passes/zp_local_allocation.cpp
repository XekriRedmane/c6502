// Per-function body-local zero-page pools.
//
// `zp_slot_allocation` hands each zp_abi function a private byte range
// for its parameters, disjoint from every transitive caller's range.
// `allocate_function_locals` extends the same call-graph analysis to
// body locals, the bytes the asm register allocator colors for pseudos
// that aren't parameters, statics, or spilled. Its pools are disjoint
// from every transitive caller's pool and from the parameter slots of
// every zp_abi function that can be on the call stack at the same time.
// Two functions on paths that don't overlap (neither calls the other,
// directly or not) may share addresses, since their activations are
// never on the call stack together. Only the eligibility is ported so
// far, which `select_abi`, `find_shares`, and the stage's driver read.
//
// Eligibility
//
// A function is eligible for the private-pool model when
//
//   1. it's defined in this unit, so that its body shows its callees and
//      its demand for local bytes;
//   2. its body contains no `IndirectCall`, since an indirect callee's
//      writes can't be bounded;
//   3. it isn't on any cycle of the static direct call graph, since a
//      recursive call would overwrite the outer activation's locals that
//      are still live. The exception is a cycle whose every call from
//      one member to another is marked
//      `[[c6502::save_hardware_stack]]` (`hw_stack_cycles`): the caller
//      saves what's live across each of those calls on the hardware
//      stack, so nothing in its pool has to survive them;
//   4. every direct callee defined in this unit is eligible too, since
//      an ineligible callee's writes are unbounded and the caller's pool
//      couldn't be trusted to survive the call;
//   5. every direct callee declared `extern` here is zp_abi. Such an
//      extern is taken as a leaf whose writes are its parameter slots,
//      as its annotation promises, since its body isn't visible. A
//      callee that is neither disqualifies the caller.
//
// A call to a `[[noreturn]]` function that passes no arguments isn't an
// edge of the call graph in rules 3, 4, and 5 (call_edges.h). Once it's
// made, nothing reads the caller's pool or the pools of the functions
// the caller was called from, so the callee may be anything, even a
// function that calls the caller again.
//
// An ineligible function gets no pool, and its register allocation uses
// the conservative caller-saved and callee-saved partition of `Pool`.
#include "passes/zp_local_allocation.h"

#include <deque>
#include <string>

#include "asdl/runtime.h"
#include "passes/call_edges.h"

namespace c6502::passes {

namespace tac = ::c6502::tac_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace zp_local_allocation {

NameSet determine_eligibility(const tac::Program& prog, const Abi& abi, const CallGraph& callgraph) {
  NameSet has_indirect;
  NameSet has_non_zp_abi_extern_callee;
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto& instr : fn->instructions) {
      if (isa<tac::IndirectCall>(instr)) {
        has_indirect.insert(fn->name);
      } else if (const auto* call = as<tac::FunctionCall>(instr)) {
        if (callgraph.count(call->name) != 0 || abandons_caller(instr)) {
          continue;
        }
        // An external callee, which is acceptable when it's declared
        // zp_abi, since its annotation bounds its writes to its
        // parameter slots.
        const ParamLayout* layout = abi.find(call->name);
        if (layout == nullptr || !std::holds_alternative<ZpLayout>(*layout)) {
          has_non_zp_abi_extern_callee.insert(fn->name);
        }
      }
    }
  }
  NameSet ineligible = has_indirect;
  ineligible.insert(has_non_zp_abi_extern_callee.begin(), has_non_zp_abi_extern_callee.end());
  const Cycles all_cycles = cycles(callgraph);
  const Cycles saved = hw_stack_cycles(prog);
  for (const auto& [n, members] : all_cycles) {
    if (!saved.contains(n)) {
      ineligible.insert(n);
    }
  }
  // A caller of an ineligible function is ineligible, since its private
  // pool can't survive the callee's unbounded writes.
  const CallGraph callers_of = invert(callgraph);
  std::deque<std::string> queue(ineligible.begin(), ineligible.end());
  while (!queue.empty()) {
    const std::string n = queue.front();
    queue.pop_front();
    const auto it = callers_of.find(n);
    if (it == callers_of.end()) {
      continue;
    }
    for (const auto& caller : it->second) {
      if (ineligible.insert(caller).second) {
        queue.push_back(caller);
      }
    }
  }
  NameSet out;
  for (const auto& [n, callees] : callgraph) {
    if (ineligible.count(n) == 0) {
      out.insert(n);
    }
  }
  return out;
}

CallGraph invert(const CallGraph& callgraph) {
  CallGraph out;
  for (const auto& [n, callees] : callgraph) {
    out[n];
  }
  for (const auto& [caller, callees] : callgraph) {
    for (const auto& callee : callees) {
      out[callee].insert(caller);
    }
  }
  return out;
}

}  // namespace zp_local_allocation

NameSet pool_eligible(const tac::Program& prog, const Abi& abi) {
  return zp_local_allocation::determine_eligibility(prog, abi, call_graph(prog));
}

Cycles recursive_pool_functions(const tac::Program& prog, const Abi& abi) {
  const NameSet eligible = zp_local_allocation::determine_eligibility(prog, abi, call_graph(prog));
  Cycles out;
  for (const auto& [n, members] : hw_stack_cycles(prog)) {
    if (eligible.count(n) != 0) {
      out.insert_or_assign(n, members);
    }
  }
  return out;
}

}  // namespace c6502::passes
