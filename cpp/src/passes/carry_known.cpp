// Drops a `SEC` or `CLC` where the carry already has that value.
//
// What it does
//
// A forward dataflow over the function finds, before each instruction,
// whether C is known set, known clear, or unknown. `SEC` and `CLC` set
// it, `BCS` and `BCC` tell each of their two edges which way it went,
// and an instruction that writes C (`ADC`, `SBC`, a compare, a shift or
// rotate, a call, and anything else not known to leave C alone) makes it
// unknown. Then:
//
//   * An `ADC #0` where C is known clear, or an `SBC #0` where it's known
//     set, goes when V is dead after it and N/Z are dead or were set from
//     A by the instruction before. It leaves A and C as they were. `P =
//     &LOCS[0]` on an array at a fixed address had kept `LDA #>LOCS; ADC
//     #0` after the low byte's `CLC`.
//   * A `SEC` where C is known set, or a `CLC` where it's known clear,
//     goes, and so does one whose carry nothing reads (`c_dead_at`), as
//     when dead-code elimination has removed the add it was for.
//   * A `SEC` (or `CLC`) that starts a block, after its labels, moves in
//     front of those labels when every path into the block other than
//     the one falling into it already has C that way. Only the
//     fall-through path, the way into a loop, then runs it.
//
// The second is the shape of `do_rwts_on_sector`'s loop
// (`examples/zork/disk.c`) once its high byte is in X:
//
//     LDX   SCRATCH1+1
//     .loop@0_continue:
//     SEC
//     SBC   SECTORS_PER_TRACK
//     BCS   .thread@0
//     CPX   #$01
//     DEX
//     BCC   .loop@0_break
//     .thread@0:
//     INC   iob+4
//     JMP   .loop@0_continue
//
// Both ways to `.thread@0` have C set, so the `JMP` back does too, and
// the `SEC` moves in front of `.loop@0_continue`, as in Zork's code.
// That saves 2 cycles a pass.
//
// Where it runs
//
// Once, after the third peephole fixed point (`compile.optimized_asm`).
// Several peepholes match a SEC or CLC: `transfer_pm1_store` turns
// `TXA; CLC; ADC #1; STA M` into `INX; STX M`, and run inside the fixed
// point this pass dropped the CLC first, which left `TXA; ADC #1`.
#include "passes/carry_known.h"

#include <cstddef>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrList = std::vector<ir::Type_instruction>;

// The state of C before an instruction: known clear (0), known set
// (1), unknown, or not reached by the walk yet.
constexpr int kUnknown = -1;
constexpr int kUnseen = -2;

// An edge into an instruction: the instruction it comes from, and the C
// it brings.
struct Edge {
  std::size_t from = 0;
  int c = kUnknown;
};

// Returns whether `instr` leaves C alone. Anything else is taken to
// change it.
bool keeps_c(const ir::Type_instruction& instr) {
  return isa<ir::Mov, ir::Inc, ir::Dec, ir::And, ir::Or, ir::Xor, ir::Push, ir::Pop,
             ir::BitTest, ir::Label, ir::Branch, ir::Jump>(instr);
}

// Returns C after `instr` falls through, given C before it.
int after(const ir::Type_instruction& instr, int c) {
  if (isa<ir::SetCarry>(instr)) {
    return 1;
  }
  if (isa<ir::ClearCarry>(instr)) {
    return 0;
  }
  return keeps_c(instr) ? c : kUnknown;
}

// Returns `(successor, C on that edge)` for each way out of `instrs[i]`,
// given C before it.
std::vector<std::pair<std::size_t, int>> edges(const InstrList& instrs, const LabelMap& labels,
                                               std::size_t i, int c) {
  const auto& ins = instrs[i];
  if (isa<ir::Return, ir::Ret>(ins)) {
    return {};
  }
  if (const auto* j = as<ir::Jump>(ins)) {
    const long at = labels.find(j->target);
    if (at < 0) {
      return {};
    }
    return {{static_cast<std::size_t>(at), c}};
  }
  if (const auto* b = as<ir::Branch>(ins)) {
    int taken = c;
    int fall = c;
    if (isa<ir::CS>(b->cond)) {
      taken = 1;
      fall = 0;
    } else if (isa<ir::CC>(b->cond)) {
      taken = 0;
      fall = 1;
    }
    std::vector<std::pair<std::size_t, int>> out;
    if (i + 1 < instrs.size()) {
      out.emplace_back(i + 1, fall);
    }
    const long at = labels.find(b->target);
    if (at >= 0) {
      out.emplace_back(static_cast<std::size_t>(at), taken);
    }
    return out;
  }
  if (i + 1 >= instrs.size()) {
    return {};
  }
  return {{i + 1, after(ins, c)}};
}

// C before each instruction, and the edges into each instruction with
// the C each brings, in the order of the instructions they come from.
struct CarryStates {
  std::vector<int> before;
  std::vector<std::vector<Edge>> incoming;
};

CarryStates carry_states(const InstrList& instrs) {
  const LabelMap labels(instrs);
  CarryStates s;
  s.before.assign(instrs.size(), kUnseen);
  s.incoming.resize(instrs.size());
  if (instrs.empty()) {
    return s;
  }
  s.before[0] = kUnknown;
  std::vector<std::size_t> work = {0};
  while (!work.empty()) {
    const std::size_t i = work.back();
    work.pop_back();
    for (const auto& [j, c] : edges(instrs, labels, i, s.before[i])) {
      const int old = s.before[j];
      if (old == kUnseen) {
        s.before[j] = c;
        work.push_back(j);
      } else if (old != kUnknown && old != c) {
        s.before[j] = kUnknown;
        work.push_back(j);
      }
    }
  }
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (s.before[i] == kUnseen) {
      continue;
    }
    for (const auto& [j, c] : edges(instrs, labels, i, s.before[i])) {
      s.incoming[j].push_back(Edge{i, c});
    }
  }
  return s;
}

// Returns the value a `SEC` (1) or `CLC` (0) gives C, or -1 for any
// other instruction.
int setter_value(const ir::Type_instruction& ins) {
  if (isa<ir::SetCarry>(ins)) {
    return 1;
  }
  if (isa<ir::ClearCarry>(ins)) {
    return 0;
  }
  return -1;
}

// Returns `instrs` without the `ADC #0` where C is known clear and the
// `SBC #0` where it's known set, when V is dead after and N/Z are dead
// or already set from A.
InstrList drop_zero_adds(InstrList instrs) {
  const CarryStates s = carry_states(instrs);
  const LabelMap labels(instrs);
  InstrList out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto& ins = instrs[i];
    const int c = s.before[i];
    bool no_op = false;
    if (const auto* x = as<ir::Add>(ins)) {
      no_op = c == 0 && is_reg_a(x->dst) && x->src == ir::Type_operand(ir::Imm{.value = 0});
    } else if (const auto* y = as<ir::Sub>(ins)) {
      no_op = c == 1 && is_reg_a(y->dst) && y->src == ir::Type_operand(ir::Imm{.value = 0});
    }
    if (no_op && v_dead_at(instrs, labels, i + 1)) {
      bool flags_ok = flags_dead_at(instrs, labels, i + 1);
      if (!flags_ok && i > 0) {
        const auto* prev = as<ir::Mov>(instrs[i - 1]);
        flags_ok = prev != nullptr && is_reg_a(prev->dst);
      }
      if (flags_ok) {
        continue;
      }
    }
    out.push_back(ins);
  }
  return out;
}

// Returns `instrs` without the SEC / CLC where C is known to have that
// value.
InstrList drop_known(InstrList instrs) {
  const CarryStates s = carry_states(instrs);
  InstrList out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const int v = setter_value(instrs[i]);
    if (v < 0 || s.before[i] == kUnseen || s.before[i] != v) {
      out.push_back(std::move(instrs[i]));
    }
  }
  return out;
}

// Returns `instrs` with each SEC / CLC that starts a block moved in front
// of its labels when every edge in but the fall-through already has C
// that way.
InstrList move_into_loops(InstrList instrs) {
  const CarryStates s = carry_states(instrs);
  std::set<std::size_t> drop;
  std::map<std::size_t, std::size_t> move;  // where a setter goes -> the setter
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const int want = setter_value(instrs[i]);
    if (want < 0 || s.before[i] == kUnseen) {
      continue;
    }
    std::size_t p = i;
    while (p > 0 && isa<ir::Label>(instrs[p - 1])) {
      --p;
    }
    if (p == i || p == 0) {
      continue;
    }
    if (isa<ir::Jump, ir::Return, ir::Ret>(instrs[p - 1])) {
      continue;
    }
    // The edges into the run of labels and the setter, less those from
    // one label to the next.
    std::size_t falls = 0;
    std::size_t others = 0;
    bool others_agree = true;
    for (std::size_t k = p; k <= i; ++k) {
      for (const Edge& e : s.incoming[k]) {
        if (p <= e.from && e.from < i) {
          continue;
        }
        if (e.from == p - 1) {
          ++falls;
        } else {
          ++others;
          others_agree = others_agree && e.c == want;
        }
      }
    }
    if (falls == 1 && others > 0 && others_agree) {
      drop.insert(i);
      move[p] = i;
    }
  }
  if (drop.empty()) {
    return instrs;
  }
  InstrList out;
  out.reserve(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto it = move.find(k); it != move.end()) {
      out.push_back(instrs[it->second]);
    }
    if (!drop.contains(k)) {
      out.push_back(instrs[k]);
    }
  }
  return out;
}

// Runs four steps, each on the result of the one before, since each
// changes what the next sees: dropping the adds and subtracts of 0 that
// leave C as it is, dropping the setters C already matches (which
// changes no value of C), moving setters in front of loops, and dropping
// the setters nothing reads. Done in one step, `CLC; LDA x; CLC; ADC y`
// lost both: the first as dead, since the second overwrites C, and the
// second as redundant after the first.
void rewrite_function(ir::Function& fn) {
  InstrList instrs = drop_zero_adds(fn.instructions);
  instrs = drop_known(std::move(instrs));
  instrs = move_into_loops(std::move(instrs));
  const LabelMap labels(instrs);
  InstrList out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (setter_value(instrs[i]) < 0 || !c_dead_at(instrs, labels, i + 1)) {
      out.push_back(instrs[i]);
    }
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_carry_known(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
