// Threads a branch on a known flag: a branch to a branch on the same
// flag goes where that one would.
//
// Pattern
//
//     Branch(c1, L)
//     ...
//     L:
//     Branch(c2, M)        # c2 tests the flag c1 tested
//     next:
//
// A path that takes the first branch arrives at L with that flag known:
// set for `BCS`, clear for `BCC`, and so on. If `c2` is `c1`, the
// second branch is taken too, so the first can go straight to M. If
// `c2` is `c1`'s opposite, it isn't, so the first can go to `next`, the
// instruction after it (a label is added there when there isn't one).
// Labels between L and the second branch don't change the flags.
//
// That saves a branch, 2 cycles, on that path. `carry_inc` leaves the
// shape in `do_rwts_on_sector` (`examples/zork/disk.c`):
//
//     BCS   .carry_inc@0       ; no borrow out of the low byte
//     CPX   #$01
//     DEX
//     .carry_inc@0:
//     BCC   .loop@0_break      ; never taken from the BCS
//     INC   iob+4
//
// becomes `BCS .thread@0` with `.thread@0:` in front of the `INC`, as
// Zork's own `BCS .inc_track` does.
//
// Where it runs
//
// In the asm peephole fixed point, after `branch_through_jump`. The
// fall-through path still reaches L, so L stays, and `dead_label_drop`
// removes it if nothing else uses it.
//
// The labels it adds are numbered across the program, since the sim's
// assembler rejects one name in two functions. Python's memo of the
// fixed point, which skips a function whose rewrite it has seen with
// the same next number, doesn't change the result and has no
// counterpart here.
#include "passes/branch_thread.h"

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/label_numbers.h"
#include "passes/long_branches.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

void rewrite_function(ir::Function& fn, asdl::Int& counter) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  const LabelMap labels(instrs);
  // The new target of each branch that changes, and the labels to add
  // after the second branches that are passed over.
  std::map<std::size_t, std::string> retarget;
  std::map<std::size_t, std::string> added;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto* ins = as<ir::Branch>(instrs[k]);
    if (ins == nullptr) {
      continue;
    }
    const long at = labels.find(ins->target);
    if (at < 0) {
      continue;
    }
    std::size_t j = static_cast<std::size_t>(at);
    while (j < instrs.size() && isa<ir::Label>(instrs[j])) {
      j += 1;
    }
    if (j >= instrs.size() || j == k) {
      continue;
    }
    const auto* second = as<ir::Branch>(instrs[j]);
    if (second == nullptr) {
      continue;
    }
    if (second->cond.index() == ins->cond.index()) {
      if (second->target != ins->target) {
        retarget[k] = second->target;
      }
    } else if (second->cond == long_branches::invert(ins->cond)) {
      if (j + 1 < instrs.size() && isa<ir::Label>(instrs[j + 1])) {
        retarget[k] = std::get<ir::Label>(instrs[j + 1]).name;
      } else {
        auto it = added.find(j);
        if (it == added.end()) {
          it = added.emplace(j, ".thread@" + asdl::int_to_string(counter)).first;
          counter += 1;
        }
        retarget[k] = it->second;
      }
    }
  }
  if (retarget.empty()) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size() + added.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto it = retarget.find(k); it != retarget.end()) {
      std::get<ir::Branch>(instrs[k]).target = it->second;
    }
    out.push_back(std::move(instrs[k]));
    if (const auto it = added.find(k); it != added.end()) {
      out.push_back(ir::Label{.name = it->second});
    }
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_branch_thread(asm_ast::Program prog) {
  asdl::Int counter = next_label_number(prog, {".thread@"});
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, counter);
    }
  }
  return prog;
}

}  // namespace c6502::passes
