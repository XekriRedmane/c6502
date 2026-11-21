/* Moves a step whose result is only copied on one edge onto that edge.

# What it does

After SSA destruction, a post-decrement in a loop's condition, as in
`do g(i); while (i-- != first);`, leaves

    Binary(Subtract, i, 1, %t)
    JumpIfCmp(NotEqual, i, first, .split)
    Ret(None)
  .split:
    Copy(%t, i)
    Jump(.loop)

The step is computed before the test, which reads the old `i`, and its
result reaches `i` only on the edge into `.split`. Both values are live
at the test, so `%t` takes a byte of its own, and the code is `LDA i;
SEC; SBC #1; STA t; LDA i; CMP first; BNE; ...; LDA t; STA i`. This
pass drops the `Binary` and puts the step in place of the `Copy`:

    JumpIfCmp(NotEqual, i, first, .split)
    Ret(None)
  .split:
    Binary(Subtract, i, 1, i)
    Jump(.loop)

The step is then in place, which the DEC peephole turns into `DEC i`.
A step whose `Copy` is on the fall-through path moves the same way.

# When it applies

- The step is `Binary(Add | Subtract | the bitwise operators, X, c,
  T)` with a constant `c`, where `X` and `T` are SSA-renamed names, so
  no store through a pointer and no call can write either one.
- `T` is written by the step alone and read by one `Copy(T, Z)` alone.
- From the step to the end of its block, no instruction writes `X`,
  and no label comes between. The block ends at a jump of any kind.
- The `Copy` is in the block the jump goes to, or in the block the
  jump falls through to, and only instructions that don't write `X`
  and aren't labels or jumps come before it there. A block reached
  through a label must have that one jump as its only predecessor. No
  other jump names the label, and the instruction before the label is
  an unconditional jump or a return, so nothing falls into it.

On every path from the step to the `Copy`, `X` then holds the value the
step read, so `Binary(op, X, c, Z)` at the `Copy` writes `Z` the value
the `Copy` wrote. `T` is read nowhere else, so dropping it changes no
other value, and every other path no longer computes a value that
nothing read. The step has no side effects, since the operators it
matches can't trap.

The counts of uses, definitions, and jumps to each label, and the
position of each label, are taken once before the walk, as the Python
pass takes them, and a step dropped later doesn't update them. */
#include "passes/optimization/step_on_edge.h"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <string_view>

#include "asdl/runtime.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::Instrs;
using Counts = std::map<std::string, std::size_t, std::less<>>;

std::size_t count_of(const Counts& counts, std::string_view name) {
  const auto it = counts.find(name);
  return it == counts.end() ? 0 : it->second;
}

bool writes(const ir::Type_instruction& instr, std::string_view name) {
  for (const auto d : defs_in(instr)) {
    if (d == name) {
      return true;
    }
  }
  return false;
}

bool conditional_jump(const ir::Type_instruction& ins) {
  return asdl::isa<ir::JumpIfTrue, ir::JumpIfFalse, ir::JumpIfCmp, ir::JumpIfMasked>(ins);
}

// Returns whether `ins` is a label, a jump of any kind, or a return,
// which end the straight run the pass looks along.
bool ends_run(const ir::Type_instruction& ins) {
  return asdl::isa<ir::Label, ir::Jump, ir::Ret>(ins) || conditional_jump(ins);
}

// Returns the target of a jump of any kind, or none.
std::optional<std::string> jump_target(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case ir::Kind_instruction::Jump:
      return std::get<ir::Jump>(ins).target;
    case ir::Kind_instruction::JumpIfTrue:
      return std::get<ir::JumpIfTrue>(ins).target;
    case ir::Kind_instruction::JumpIfFalse:
      return std::get<ir::JumpIfFalse>(ins).target;
    case ir::Kind_instruction::JumpIfCmp:
      return std::get<ir::JumpIfCmp>(ins).target;
    case ir::Kind_instruction::JumpIfMasked:
      return std::get<ir::JumpIfMasked>(ins).target;
    default:
      return std::nullopt;
  }
}

// Returns the index of `Copy(t, _)` at or after `start`, when only
// instructions that neither write `x` nor end the block come before it,
// and none otherwise.
std::optional<std::size_t> copy_before_write(const Instrs& instrs, std::size_t start,
                                             std::string_view x, std::string_view t) {
  for (std::size_t k = start; k < instrs.size(); ++k) {
    const auto& ins = instrs[k];
    if (const auto* c = std::get_if<ir::Copy>(&ins)) {
      if (const auto* v = std::get_if<ir::Var>(&c->src); v != nullptr && v->name == t) {
        return k;
      }
    }
    if (ends_run(ins) || writes(ins, x)) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

}  // namespace

ir::Function move_steps_onto_edges(ir::Function fn, const NameSet& ssa_names) {
  Instrs instrs = fn.instructions;
  Counts uses;
  Counts defs;
  for (const auto& ins : instrs) {
    for (const auto u : uses_in(ins)) {
      ++uses[std::string(u)];
    }
    for (const auto d : defs_in(ins)) {
      ++defs[std::string(d)];
    }
  }
  Counts label_refs;
  std::map<std::string, std::size_t, std::less<>> label_at;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto* l = std::get_if<ir::Label>(&instrs[k])) {
      label_at.insert_or_assign(l->name, k);
    }
    if (const auto target = jump_target(instrs[k])) {
      ++label_refs[*target];
    }
  }
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const auto* step = std::get_if<ir::Binary>(&instrs[i]);
    if (step == nullptr ||
        !asdl::isa<ir::Add, ir::Subtract, ir::BitwiseAnd, ir::BitwiseOr, ir::BitwiseXor>(
            step->op) ||
        !std::holds_alternative<ir::Var>(step->src1) ||
        !std::holds_alternative<ir::Constant>(step->src2) ||
        !std::holds_alternative<ir::Var>(step->dst)) {
      ++i;
      continue;
    }
    const std::string x = std::get<ir::Var>(step->src1).name;
    const std::string t = std::get<ir::Var>(step->dst).name;
    if (x == t || !ssa_names.contains(x) || !ssa_names.contains(t) || count_of(defs, t) != 1 ||
        count_of(uses, t) != 1) {
      ++i;
      continue;
    }
    std::size_t j = i + 1;
    while (j < instrs.size() && !ends_run(instrs[j])) {
      if (writes(instrs[j], x)) {
        break;
      }
      ++j;
    }
    if (j >= instrs.size() || asdl::isa<ir::Label, ir::Ret>(instrs[j]) || writes(instrs[j], x) ||
        !(std::holds_alternative<ir::Jump>(instrs[j]) || conditional_jump(instrs[j]))) {
      ++i;
      continue;
    }
    const auto& jump = instrs[j];
    std::optional<std::size_t> found;
    if (conditional_jump(jump)) {
      found = copy_before_write(instrs, j + 1, x, t);
    }
    if (!found) {
      const std::string target = *jump_target(jump);
      const auto at = label_at.find(target);
      if (at != label_at.end() && count_of(label_refs, target) == 1 && at->second > 0 &&
          at->second <= instrs.size() &&
          asdl::isa<ir::Jump, ir::Ret>(instrs[at->second - 1])) {
        found = copy_before_write(instrs, at->second + 1, x, t);
      }
    }
    if (!found) {
      ++i;
      continue;
    }
    const ir::Type_val copy_dst = std::get<ir::Copy>(instrs[*found]).dst;
    ir::Binary moved{.op = step->op, .src1 = step->src1, .src2 = step->src2, .dst = copy_dst};
    instrs[*found] = std::move(moved);
    instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(i));
    changed = true;
  }
  if (!changed) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(instrs));
}

ir::Function move_steps_onto_edges(ir::Function fn, const framework::PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr) {
    return fn;
  }
  return move_steps_onto_edges(std::move(fn), *ctx.ssa_dsts);
}

}  // namespace c6502::passes
