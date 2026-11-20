// Expands branches that can't reach their targets into a branch on the
// opposite condition and a `JMP`.
//
// The 6502's `Bxx` instructions (`BCC`, `BCS`, `BEQ`, `BMI`, `BNE`,
// `BPL`, `BVC`, `BVS`) carry a signed 8-bit displacement, so a branch
// reaches only a target within -128 to +127 bytes of the instruction
// after it. A function large enough to put a label outside that window
// from a branch can't encode the branch as one instruction.
//
// This pass walks each function's instructions, computes the label
// addresses, finds the branches that are too long, and rewrites each
// one into a 5-byte sequence:
//
//     Branch(cond, target)
//         becomes
//     Branch(inverted_cond, .lb_skip@N)
//     Jump(target)
//     Label(.lb_skip@N)
//
// The inverted branch skips the 3-byte `JMP` when the original
// condition is false, so falling into the `JMP` is the case where the
// original branch was taken. `JMP` is absolute, 3 bytes, and reaches
// anywhere in the 64 KB.
//
// Iteration
//
// Each expansion grows the function by 3 bytes, which can push other
// branches out of their windows, so the pass repeats on each function
// until no branch needs expanding. That ends, because the function
// only grows, and a branch once expanded stays expanded.
//
// Per function
//
// Branches always target labels in their own function. The code
// generator never crosses a function's boundary with a `Bxx`, since a
// transfer to another function is a `JSR`, an `RTS`, or a `JMP`. The
// pass works on each `Function` by itself, and a branch to a name
// outside its function raises an error.
//
// Where it runs
//
// After `replace_pseudoregisters`, so every `Pseudo` operand has become
// a `Frame`, `Stack`, or `Data` and each instruction's size is known,
// and before `asm_emit.emit_program` or `sim.assembler.assemble`. Both
// see the same expanded program.
//
// The skip labels follow the dasm local-label convention used
// elsewhere, `.lb_skip@<N>`, where `N` counts across the program. The
// `@` keeps them apart from user labels (`.<funcname>@<orig>`) and from
// the other labels the translator makes (`.if_end@<N>` and so on).
#include "passes/long_branches.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "errors.h"
#include "sim/assembler.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// Returns the byte offset of each label in the function, where the
// function starts at 0 and each instruction other than a label
// advances the offset by its size.
std::unordered_map<std::string, long> label_addresses(
    const std::vector<ir::Type_instruction>& instrs) {
  std::unordered_map<std::string, long> addr_map;
  long addr = 0;
  for (const auto& instr : instrs) {
    if (const auto* label = as<ir::Label>(instr)) {
      addr_map[label->name] = addr;
    } else {
      addr += sim::instruction_size(instr);
    }
  }
  return addr_map;
}

// Returns the indices of the branches whose displacement to their
// target is outside the signed 8-bit range. The 6502's PC has passed
// the 2-byte branch when the displacement is added, so it is `target -
// (branch_addr + 2)`.
std::vector<std::size_t> find_too_far_branches(
    const std::vector<ir::Type_instruction>& instrs,
    const std::unordered_map<std::string, long>& label_addr) {
  std::vector<std::size_t> out;
  long addr = 0;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto& instr = instrs[i];
    if (const auto* br = as<ir::Branch>(instr)) {
      const auto it = label_addr.find(br->target);
      if (it == label_addr.end()) {
        throw InternalError(
            "Branch target '" + br->target +
            "' not found in this function — branches must stay within "
            "their containing function");
      }
      const long disp = it->second - (addr + 2);
      if (disp < -128 || disp > 127) {
        out.push_back(i);
      }
    }
    if (!isa<ir::Label>(instr)) {
      addr += sim::instruction_size(instr);
    }
  }
  return out;
}

// Replaces each branch at the indices `too_far` with the long-branch
// triple, from the back so that the earlier indices stay valid. Each
// expansion makes a new `.lb_skip@<N>` label.
std::vector<ir::Type_instruction> expand_indices(
    const std::vector<ir::Type_instruction>& instrs,
    const std::vector<std::size_t>& too_far, long& counter) {
  std::vector<std::size_t> expand_at(instrs.size(), 0);
  std::vector<std::string> skip_label(instrs.size());
  for (auto it = too_far.rbegin(); it != too_far.rend(); ++it) {
    expand_at[*it] = 1;
    skip_label[*it] = ".lb_skip@" + std::to_string(counter++);
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size() + 2 * too_far.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (!expand_at[i]) {
      out.push_back(instrs[i]);
      continue;
    }
    const auto& br = std::get<ir::Branch>(instrs[i]);
    out.push_back(ir::Branch{.cond = long_branches::invert(br.cond),
                             .target = skip_label[i]});
    out.push_back(ir::Jump{.target = br.target});
    out.push_back(ir::Label{.name = skip_label[i]});
  }
  return out;
}

// Expands branches until none is too far, and returns the function's
// new instructions. `counter` numbers the skip labels.
std::vector<ir::Type_instruction> expand_function(
    std::vector<ir::Type_instruction> instrs, long& counter) {
  while (true) {
    const auto label_addr = label_addresses(instrs);
    const auto too_far = find_too_far_branches(instrs, label_addr);
    if (too_far.empty()) {
      return instrs;
    }
    instrs = expand_indices(instrs, too_far, counter);
  }
}

}  // namespace

namespace long_branches {

asm_ast::Type_condition invert(const asm_ast::Type_condition& cond) {
  // Each pair flips the sense of a `Bxx`, so the new branch falls
  // through to the `JMP` when the original would have been taken.
  switch (ir::kind(cond)) {
    case ir::Kind_condition::CC:
      return ir::CS{};
    case ir::Kind_condition::CS:
      return ir::CC{};
    case ir::Kind_condition::EQ:
      return ir::NE{};
    case ir::Kind_condition::NE:
      return ir::EQ{};
    case ir::Kind_condition::MI:
      return ir::PL{};
    case ir::Kind_condition::PL:
      return ir::MI{};
    case ir::Kind_condition::VC:
      return ir::VS{};
    case ir::Kind_condition::VS:
      return ir::VC{};
  }
  throw InternalError("unknown condition");
}

}  // namespace long_branches

asm_ast::Program expand_long_branches(asm_ast::Program prog) {
  long counter = 0;
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      fn->instructions = expand_function(std::move(fn->instructions), counter);
    }
  }
  return prog;
}

}  // namespace c6502::passes
