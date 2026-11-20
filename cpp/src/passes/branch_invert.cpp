// Inverts a branch around a jump.
//
// `tac_to_asm` lowers `if (cond) JumpIfFalse target` and similar
// shapes as a conditional branch around an unconditional jump:
//
//     Branch(cond, L)
//     Jump(target)
//     Label(L)
//
// The branch skips the jump when `cond` is true. Otherwise control
// falls into the jump and lands at `target`. The inverted form has no
// `JMP`, so it is shorter, and it is faster on the taken path (a branch
// is 2 bytes and 2 or 3 cycles, a `JMP` 3 bytes and 3 cycles):
//
//     Branch(!cond, target)
//     Label(L)
//
// `Label(L)` stays, since other instructions may jump to it, and the
// inversion is sound whatever else names it. `dead_label_drop` removes
// it if nothing does.
//
// `tac_to_asm` emits this pattern for every C `if` whose condition has
// the wrong polarity to fall through directly, so the peephole fires
// across most control flow.
//
// Soundness
//
//   * `cond` and `!cond` hold for disjoint sets of flag states (BCC
//     branches if and only if C=0, BCS if and only if C=1, and the same
//     for the other three pairs), so a branch on `!cond` to `target`
//     goes where a branch on `cond` past the `JMP` and the `JMP` went.
//   * A `Branch` reads and writes no operand. It reads only the flags,
//     which the rewrite leaves as they were.
//
// Where it runs
//
// In the asm peephole fixed point, after `replace_pseudoregisters` and
// before `expand_long_branches`. The inverted branch's target may be
// further away than `L` was, so `expand_long_branches` has to run
// after it, as it does.
//
// The rewrite turns three atoms into two and never grows the code,
// which the fixed point's shrinking relies on.
#include "passes/branch_invert.h"

#include <cstddef>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "passes/long_branches.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;

void rewrite_function(ir::Function& fn) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  // A Jump to a name that isn't a label here is a tail call to another
  // function. A branch can't reach it, since its distance isn't known
  // until the link, and `expand_long_branches` measures only branches
  // within one function.
  std::unordered_set<std::string> local_labels;
  for (const auto& ins : instrs) {
    if (const auto* label = as<ir::Label>(ins)) {
      local_labels.insert(label->name);
    }
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  std::size_t i = 0;
  while (i + 2 < instrs.size()) {
    const auto* a = as<ir::Branch>(instrs[i]);
    const auto* b = as<ir::Jump>(instrs[i + 1]);
    const auto* c = as<ir::Label>(instrs[i + 2]);
    if (a != nullptr && b != nullptr && c != nullptr && a->target == c->name &&
        local_labels.contains(b->target)) {
      out.push_back(ir::Branch{.cond = long_branches::invert(a->cond),
                               .target = b->target});
      out.push_back(std::move(instrs[i + 2]));
      i += 3;
      continue;
    }
    out.push_back(std::move(instrs[i]));
    i += 1;
  }
  while (i < instrs.size()) {
    out.push_back(std::move(instrs[i]));
    i += 1;
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_branch_invert(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
