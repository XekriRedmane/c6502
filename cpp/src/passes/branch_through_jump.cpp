// Merges a branch or jump into the jump at its target:
// `Branch(cond, L); ...; L: Jump(T)` becomes `Branch(cond, T); ...`,
// and the same for an unconditional `Jump(L)`.
//
// Motivating shape (the tail of snd_delay_up's outer loop):
//
//     DEX
//     BNE .ssa_split@0
//     RTS
//     .ssa_split@0:
//     JMP .loop_start
//
// The conditional branch lands on a label whose code is only an
// unconditional jump. The compiler emits this when the natural fall
// through after the branch is the function's exit and the loop's
// continuation is too far for a short `BNE`, so every iteration pays
// for a 3-byte, 3-cycle trampoline.
//
// Retargeting the branch to the jump's target, `BNE .loop_start; RTS`,
// takes the trampoline off the branch's path. The label and the jump
// are then dead, and they are dropped when nothing else names `L`.
//
// Soundness
//
//   * Before, `Branch(cond, L)` either goes to `L`, which jumps to `T`,
//     or falls through into the instructions between the branch and
//     the label.
//   * After, `Branch(cond, T)` either goes to `T` directly or falls
//     through into the same instructions. The fall-through never
//     reaches `L` when the branch was its only way in, so the label and
//     the jump can go when nothing else names `L`.
//   * The branch's condition, the instructions in between, and the
//     jump's target are all as they were. Only the trampoline goes.
//
// When the instructions in between can fall into the label (they
// don't end in a return or a jump), dropping the label and the jump
// would change what the program does. The branch is still retargeted,
// which only removes the trampoline from its path, and the label and
// the jump stay for the fall-through. `apply_dead_label_drop` removes
// them if a later rewrite leaves them with no reference.
//
// An unconditional `Jump(L)` is retargeted the same way. The TAC
// optimizer folds `if (0)` to a `Jump`, so `goto_backwards.c` came out
// as
//
//     JMP .if_end@0
//     .main@label:
//     LDX #$00
//     LDA #$05
//     RTS
//     .if_end@0:
//     JMP .main@label
//
// and became `JMP .main@label` straight into `.main@label`, which
// `branch_to_next_drop` deletes, and the `.if_end@0: JMP` that nothing
// names goes too.
//
// A chain that loops back on itself (`L1: JMP L2; L2: JMP L1`, an
// endless loop) is left alone, since retargeting inside the cycle would
// keep changing it and the peephole loop wouldn't settle.
//
// Where it runs
//
// In the asm peephole fixed point, after `apply_branch_invert`.
// Together they collapse the two trampoline shapes `tac_to_asm` emits
// for loop tails.
#include "passes/branch_through_jump.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// Returns whether `instr` transfers control away unconditionally, so
// nothing falls through to the next instruction.
bool is_terminator(const ir::Type_instruction& instr) {
  return isa<ir::Jump, ir::Ret, ir::Return>(instr);
}

// Returns whether `instr` is a `Mov` whose source and destination are
// the same (the same register, or the same `Data` or `ZP` byte).
// `asm_emit` drops these when it writes the code, and the pass looks
// past them when it matches a pattern too.
bool is_noop_mov(const ir::Type_instruction& instr) {
  const auto* mov = as<ir::Mov>(instr);
  if (mov == nullptr || mov->is_volatile) {
    return false;
  }
  return mov->src == mov->dst;
}

// Calls `fn` on each label name that `instr`'s control flow names.
template <class Fn>
void for_each_label_ref(const ir::Type_instruction& instr, Fn&& fn) {
  if (const auto* branch = as<ir::Branch>(instr)) {
    fn(branch->target);
  } else if (const auto* jump = as<ir::Jump>(instr)) {
    fn(jump->target);
  } else if (const auto* phi = as<ir::Phi>(instr)) {
    for (const auto& arg : phi->args) {
      fn(std::get<ir::AsmPhiArg>(arg).pred_label);
    }
  }
}

void rewrite_function(ir::Function& fn) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  // The function's own labels, which are the only names a `Branch`
  // can target, since a 6502 branch is relative and can't leave the
  // function. A tail call's `Jump` names another function and is never
  // made a branch's target.
  std::unordered_set<std::string> in_fn_labels;
  for (const auto& instr : instrs) {
    if (const auto* label = as<ir::Label>(instr)) {
      in_fn_labels.insert(label->name);
    }
  }
  // Each label whose next instruction that does something is an
  // unconditional `Jump`, mapped to that jump's target. A label
  // followed by another label goes on to whatever the inner one
  // resolves to.
  std::unordered_map<std::string, std::string> label_to_jump_target;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto* label = as<ir::Label>(instrs[i]);
    if (label == nullptr) {
      continue;
    }
    // The walk passes labels in a row and moves of an operand to
    // itself (the `Mov(R, R)` that `asm_emit` drops but that stays in
    // the IR, which SSA destruction's parallel copies leave when a
    // merge made the copy a self-copy). The first instruction after
    // those decides the shape.
    std::size_t j = i + 1;
    while (j < instrs.size() &&
           (isa<ir::Label>(instrs[j]) || is_noop_mov(instrs[j]))) {
      j += 1;
    }
    if (j >= instrs.size()) {
      continue;
    }
    const auto* next = as<ir::Jump>(instrs[j]);
    if (next != nullptr && in_fn_labels.contains(next->target)) {
      label_to_jump_target[label->name] = next->target;
    }
  }
  if (label_to_jump_target.empty()) {
    return;
  }
  // Resolves a chain: a label that jumps to a label that jumps to `T`
  // goes to `T`. A chain that comes back to a label it passed is an
  // endless loop, and is left as it is.
  const auto resolve = [&](const std::string& target) -> std::string {
    std::unordered_set<std::string> seen{target};
    std::string cur = target;
    while (true) {
      const auto it = label_to_jump_target.find(cur);
      if (it == label_to_jump_target.end()) {
        return cur;
      }
      if (seen.contains(it->second)) {
        return target;
      }
      seen.insert(it->second);
      cur = it->second;
    }
  };
  // The references to each label, from branches, jumps, and Phi
  // arguments, which decide which trampolines can go after the
  // retargeting. The function's name isn't a label and doesn't count.
  std::unordered_map<std::string, int> ref_count;
  for (const auto& instr : instrs) {
    for_each_label_ref(instr, [&](const std::string& ref) { ++ref_count[ref]; });
  }
  // First, the branches and jumps whose label holds only a jump are
  // retargeted, counting the references each label loses.
  std::vector<ir::Type_instruction> new_instrs;
  new_instrs.reserve(instrs.size());
  std::unordered_map<std::string, int> drop_decremented;
  for (auto& instr : instrs) {
    std::string* target = nullptr;
    if (auto* branch = as<ir::Branch>(instr)) {
      target = &branch->target;
    } else if (auto* jump = as<ir::Jump>(instr)) {
      target = &jump->target;
    }
    if (target != nullptr && label_to_jump_target.contains(*target)) {
      std::string new_target = resolve(*target);
      if (new_target != *target) {
        ++drop_decremented[*target];
        *target = std::move(new_target);
      }
    }
    new_instrs.push_back(std::move(instr));
  }
  // Then each `Label; (no-op Mov)*; Jump` trampoline goes whose label
  // has no reference left and which nothing falls into, since the
  // instruction before it is a `Return`, a `Ret`, or a `Jump`. Without
  // that test, a label between two reachable blocks would go with the
  // no-op moves and the jump that the block before it falls into.
  std::unordered_set<std::string> fully_drop;
  for (const auto& [name, dec] : drop_decremented) {
    const auto it = ref_count.find(name);
    const int refs = it == ref_count.end() ? 0 : it->second;
    if (refs - dec <= 0) {
      fully_drop.insert(name);
    }
  }
  if (fully_drop.empty()) {
    instrs = std::move(new_instrs);
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(new_instrs.size());
  std::size_t i = 0;
  while (i < new_instrs.size()) {
    const auto* label = as<ir::Label>(new_instrs[i]);
    if (label != nullptr && fully_drop.contains(label->name) && !out.empty() &&
        is_terminator(out.back())) {
      // The label goes, with the no-op moves and the labels with no
      // reference after it, up to and including the closing jump.
      i += 1;
      while (i < new_instrs.size()) {
        const auto& next = new_instrs[i];
        if (isa<ir::Jump>(next)) {
          i += 1;
          break;
        }
        if (const auto* inner = as<ir::Label>(next)) {
          if (fully_drop.contains(inner->name)) {
            i += 1;
            continue;
          }
          break;
        }
        if (is_noop_mov(next)) {
          i += 1;
          continue;
        }
        break;
      }
      continue;
    }
    out.push_back(std::move(new_instrs[i]));
    i += 1;
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_branch_through_jump(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
