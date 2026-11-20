// Makes jumps shorter once branches have their final reach: a jump to a
// stub that only jumps to another function goes there itself, the stub
// goes, and a `JMP` right after a load of a constant becomes a branch
// when its label is in reach.
//
// Pattern
//
// A switch's default is often a call that never returns, which
// `tail_call` leaves as a stub `.default@N: JMP internal_error`. The
// bounds check of a jump table branches there, and
// `expand_long_branches` makes that `BCC skip; JMP .default@N` when the
// stub is far away, as it is at the end of a long switch:
//
//     CMP   #$10                       CMP   #$10
//     BCC   .lb_skip@0                 BCC   .lb_skip@0
//     JMP   .default@33       ->       JMP   internal_error
//     ...                              ...
//     .jtab@1:                         .jtab@1:
//     DC.W  .default@33-1              DC.W  internal_error-1
//     ...                              ...
//     .default@33:
//     JMP   internal_error
//
// The `JMP`s and the table's entries for the default go to the
// function the stub jumps to, and the stub, which nothing reaches any
// more, goes, 3 bytes.
//
// `tac_to_asm` lowers a comparison kept as a value as `Bcc t; LDA #0;
// JMP end; t: LDA #1; end:`. `LDA #0` sets Z, so `JMP end` is `BEQ end`
// when `end` is within a branch's reach, 1 byte shorter. A load of a
// constant other than 0 clears Z, so its `JMP` is `BNE`. The same goes
// for `LDX` and `LDY`.
//
// A `JMP` right after a conditional branch, with no label between, runs
// only when the branch isn't taken. The branch's condition is false
// there, so the branch on the opposite condition is always taken. A
// jump threaded past a test on the TAC
// (`optimization/known_compare_thread.py`) leaves `BEQ .a; JMP .b`
// where neither place comes next, and the `JMP` becomes `BNE .b`.
//
// Rules
//
// - A `JMP .L`, with no register arguments, where the first instruction
//   at `.L` and the labels with it is a `JMP f` to a name outside the
//   function (a tail call) with none either, becomes `JMP f`. So does
//   an entry `.L` of a jump table.
// - A label that no branch, jump, jump table, or operand names any
//   more, after an instruction that doesn't fall through, goes, and so
//   does the code after it up to the next label.
// - `LDA #k`, `LDX #k`, or `LDY #k` and then `JMP .L` to a label in the
//   function: the `JMP` becomes `BEQ .L` for `k` of 0 and `BNE .L`
//   otherwise, when the branch reaches `.L` with a margin of a byte
//   each way. So does a `JMP .L` right after a conditional branch,
//   which becomes the branch on the opposite condition. The
//   displacements come from the code as it stands, and every rewrite
//   here only removes bytes, so a branch that reaches its label still
//   does after the others.
//
// Where it runs
//
// Once, at the end of the `-O` asm pipeline, right after
// `expand_long_branches`, when the code's layout is final apart from
// what this pass removes.
#include "passes/short_jumps.h"

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "passes/long_branches.h"
#include "sim/assembler.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

bool is_terminator(const ir::Type_instruction& ins) {
  return isa<ir::Jump, ir::Return, ir::Ret, ir::JumpTable>(ins);
}

// Returns the function a stub at label `name` jumps to, when the first
// instruction there is a `JMP` out of the function.
std::optional<std::string> tail_call_at(const Instructions& instrs,
                                        const LabelMap& labels,
                                        const std::string& name) {
  const long found = labels.find(name);
  if (found < 0) {
    return std::nullopt;
  }
  std::size_t k = static_cast<std::size_t>(found);
  while (k < instrs.size() && isa<ir::Label>(instrs[k])) {
    k += 1;
  }
  if (k < instrs.size()) {
    const auto* jump = as<ir::Jump>(instrs[k]);
    if (jump != nullptr && jump->reg_args.empty() &&
        !jump->target.starts_with(".")) {
      return jump->target;
    }
  }
  return std::nullopt;
}

Instructions thread_to_tail_calls(const Instructions& instrs) {
  const LabelMap labels(instrs);
  Instructions out;
  out.reserve(instrs.size());
  for (const auto& ins : instrs) {
    if (const auto* jump = as<ir::Jump>(ins)) {
      if (jump->reg_args.empty() && jump->target.starts_with(".")) {
        if (auto f = tail_call_at(instrs, labels, jump->target)) {
          out.push_back(ir::Jump{.target = std::move(*f)});
          continue;
        }
      }
    } else if (const auto* table = as<ir::JumpTable>(ins)) {
      ir::JumpTable copy = *table;
      for (auto& t : copy.targets) {
        if (auto f = tail_call_at(instrs, labels, t)) {
          t = std::move(*f);
        }
      }
      out.push_back(std::move(copy));
      continue;
    }
    out.push_back(ins);
  }
  return out;
}

// Returns every label a branch, jump, jump table, or operand names.
std::unordered_set<std::string> referenced(const Instructions& instrs) {
  std::unordered_set<std::string> out;
  for (const auto& ins : instrs) {
    if (const auto* jump = as<ir::Jump>(ins)) {
      out.insert(jump->target);
    } else if (const auto* branch = as<ir::Branch>(ins)) {
      out.insert(branch->target);
    } else if (const auto* table = as<ir::JumpTable>(ins)) {
      out.insert(table->targets.begin(), table->targets.end());
    }
    // Python reads the `name` of every value in the instruction's
    // fields, and of every element of a list field, which are the
    // named operands, a `DecimalArith`'s among them. The arguments of a
    // `Phi` have no `name`.
    const auto add_name = [&](const ir::Type_operand& op) {
      std::visit(
          [&](const auto& o) {
            if constexpr (requires { o.name; }) {
              out.insert(o.name);
            }
          },
          op);
    };
    if (isa<ir::Phi>(ins)) {
      for_each_operand<false>(ins, add_name);
    } else {
      for_each_operand<true>(ins, add_name);
    }
  }
  return out;
}

Instructions drop_dead_stubs(Instructions instrs) {
  const auto used = referenced(instrs);
  Instructions out;
  out.reserve(instrs.size());
  bool dropping = false;
  for (auto& ins : instrs) {
    if (const auto* label = as<ir::Label>(ins)) {
      const bool dead = !used.contains(label->name) && !out.empty() &&
                        (dropping || is_terminator(out.back()));
      if (dead) {
        dropping = true;
        continue;
      }
      dropping = false;
    }
    if (dropping) {
      continue;
    }
    out.push_back(std::move(ins));
  }
  return out;
}

Instructions constant_jumps_to_branches(Instructions instrs) {
  std::unordered_map<std::string, long> labels;
  std::vector<long> addrs;
  addrs.reserve(instrs.size());
  long addr = 0;
  for (const auto& ins : instrs) {
    addrs.push_back(addr);
    if (const auto* label = as<ir::Label>(ins)) {
      labels[label->name] = addr;
    } else {
      addr += sim::instruction_size(ins);
    }
  }
  std::vector<std::optional<ir::Branch>> rewrite(instrs.size());
  for (std::size_t k = 1; k < instrs.size(); ++k) {
    const auto& load = instrs[k - 1];
    const auto* jump = as<ir::Jump>(instrs[k]);
    if (!(jump != nullptr && jump->reg_args.empty() &&
          labels.contains(jump->target))) {
      continue;
    }
    ir::Type_condition cond;
    const auto* mov = as<ir::Mov>(load);
    const ir::Imm* imm = mov != nullptr ? as<ir::Imm>(mov->src) : nullptr;
    if (imm != nullptr && isa<ir::Reg>(mov->dst)) {
      cond = (imm->value & 0xFF) == 0 ? ir::Type_condition{ir::EQ{}}
                                      : ir::Type_condition{ir::NE{}};
    } else if (const auto* branch = as<ir::Branch>(load)) {
      // The jump runs only when the branch before it isn't taken, so
      // that branch's condition is false here and the opposite one is
      // true.
      cond = long_branches::invert(branch->cond);
    } else {
      continue;
    }
    // The displacement as a 2-byte branch here, with a byte to spare
    // each way for the byte it saves.
    const long disp = labels.at(jump->target) - (addrs[k] + 2);
    if (disp < -127 || disp > 126) {
      continue;
    }
    rewrite[k] = ir::Branch{.cond = cond, .target = jump->target};
  }
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (rewrite[k]) {
      instrs[k] = std::move(*rewrite[k]);
    }
  }
  return instrs;
}

}  // namespace

asm_ast::Program apply_short_jumps(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      Instructions instrs = thread_to_tail_calls(fn->instructions);
      instrs = drop_dead_stubs(std::move(instrs));
      fn->instructions = constant_jumps_to_branches(std::move(instrs));
    }
  }
  return prog;
}

}  // namespace c6502::passes
