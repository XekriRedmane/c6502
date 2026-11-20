// Cross jumping: two blocks that end in the same instructions and a
// jump to the same place share one copy of those instructions.
//
// Pattern
//
//     BNE   .x4
//     ASL   n               ; the * 2 path
//     ROL   n+1
//     JMP   .end
// .x4:
//     ...
//     ASL   n               ; the * 4 path
//     ROL   n+1
//     ASL   n
//     ROL   n+1
//     JMP   .end
//
// becomes
//
//     BNE   .x4
// .xjump@0:
//     ASL   n
//     ROL   n+1
//     JMP   .end
// .x4:
//     ...
//     ASL   n
//     ROL   n+1
//     JMP   .xjump@0
//
// The second path runs the same instructions it ran before, and then
// the same jump, so nothing it computes changes. It takes the 3 cycles
// of one more `JMP`, and the shared instructions' bytes go. Zork's
// `instr_mul` and `instr_div` (examples/zork/numbers.c) have this
// shape, as `if (b == 2) a <<= 1; else if (b == 4) a <<= 2;` leaves it,
// and the original lets the `* 4` path fall into the `* 2` one.
//
// Rules
//
// - Both blocks end in a `JMP` to the same place: the same function (a
//   tail call, with the same register arguments), or labels in the
//   function that name the same instruction.
// - The shared instructions are the longest run before the two jumps
//   that is equal instruction by instruction, with no label, branch,
//   jump, or return in it, so no other path enters either run partway.
// - The run is at least two instructions. One, such as the `LDA #1`
//   that ends many of the comparisons in Zork's `do_2op`, saves 2 bytes
//   for a 3-cycle `JMP`, and on the interpreter's hottest paths that
//   came to 52,000 more cycles in a session for 59 bytes.
// - The later block jumps into the earlier one, which gets a label in
//   front of the run (`.xjump@N`) when it has none.
//
// Each call shares the pair with the most instructions in a function,
// then looks again, until no pair is left.
//
// Where it runs
//
// In the asm peephole fixed point, after `tail_merge`, which shares a
// repeated return tail by inverting a branch around it.
//
// The labels it adds are numbered across the program, since the sim's
// assembler rejects one name in two functions. Python's memo of the
// fixed point doesn't change the result and has no counterpart here.
#include "passes/cross_jump.h"

#include <cstddef>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/label_numbers.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_stop(const ir::Type_instruction& ins) {
  return isa<ir::Label, ir::Branch, ir::Jump, ir::Return, ir::Ret>(ins);
}

// Where a jump goes: the index of the first instruction after a label
// in the function (`here`), or the name of a function it tail-calls,
// with its register arguments.
struct Destination {
  bool here = false;
  std::size_t at = 0;
  std::string name;
  std::vector<std::string> reg_args;
  bool operator==(const Destination&) const = default;
};

Destination destination(const std::vector<ir::Type_instruction>& instrs,
                        const LabelMap& labels, const ir::Jump& jump) {
  const long found = labels.find(jump.target);
  if (found < 0) {
    return Destination{.here = false, .name = jump.target,
                       .reg_args = jump.reg_args};
  }
  std::size_t at = static_cast<std::size_t>(found);
  while (at < instrs.size() && isa<ir::Label>(instrs[at])) {
    at += 1;
  }
  return Destination{.here = true, .at = at};
}

// Shares the pair of runs that has the most instructions, and returns
// whether there was one.
bool share_one(ir::Function& fn, asdl::Int& counter) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<std::pair<std::size_t, Destination>> jumps;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto* jump = as<ir::Jump>(instrs[k])) {
      jumps.emplace_back(k, destination(instrs, labels, *jump));
    }
  }
  std::optional<std::tuple<std::size_t, std::size_t, std::size_t>> best;
  for (std::size_t a = 0; a < jumps.size(); ++a) {
    const std::size_t i = jumps[a].first;
    for (std::size_t b = a + 1; b < jumps.size(); ++b) {
      const std::size_t j = jumps[b].first;
      if (!(jumps[a].second == jumps[b].second)) {
        continue;
      }
      std::size_t n = 0;
      // `i - 1 - n >= 0 and j - 1 - n > i`, with unsigned indices.
      while (n + 1 <= i && j > i + 1 + n && !is_stop(instrs[i - 1 - n]) &&
             instrs[i - 1 - n] == instrs[j - 1 - n]) {
        n += 1;
      }
      if (n >= 2 && (!best || n > std::get<2>(*best))) {
        best = std::make_tuple(i, j, n);
      }
    }
  }
  if (!best) {
    return false;
  }
  const auto [i, j, n] = *best;
  const std::size_t start = i - n;
  std::string name;
  std::optional<std::size_t> new_label;
  if (start > 0 && isa<ir::Label>(instrs[start - 1])) {
    name = std::get<ir::Label>(instrs[start - 1]).name;
  } else {
    name = ".xjump@" + asdl::int_to_string(counter);
    counter += 1;
    new_label = start;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size() + 1);
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (new_label && k == *new_label) {
      out.push_back(ir::Label{.name = name});
    }
    if (j - n <= k && k < j) {
      continue;
    }
    if (k == j) {
      out.push_back(ir::Jump{.target = name});
      continue;
    }
    out.push_back(std::move(instrs[k]));
  }
  instrs = std::move(out);
  return true;
}

}  // namespace

asm_ast::Program apply_cross_jump(asm_ast::Program prog) {
  asdl::Int counter = next_label_number(prog, {".xjump@"});
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      // Pairs are shared until none is left. Each one removes
      // instructions, so this ends, and a function with many repeated
      // tails, such as the `LDA #1; JMP` of a switch's comparisons in
      // Zork's `do_2op`, doesn't take a sweep of the fixed point for
      // each.
      while (share_one(*fn, counter)) {
      }
    }
  }
  return prog;
}

}  // namespace c6502::passes
