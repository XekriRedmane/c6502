// Moves a run of instructions that several places in a unit repeat into
// a function of its own, which each place calls.
//
// Pattern
//
// Functions that call the same routine with the same arguments repeat
// the code that sets the arguments up. Zork's `instr_mul`, `instr_div`,
// and `instr_mod` (examples/zork/numbers.c) each start
//
//     LDA   OPERANDS+2
//     STA   __zpabi_check_sign__b_0
//     LDA   OPERANDS+3
//     STA   __zpabi_check_sign__b_1
//     LDA   OPERANDS
//     STA   __zpabi_check_sign__a_0
//     LDA   OPERANDS+1
//     STA   __zpabi_check_sign__a_1
//     JSR   check_sign
//
// 19 bytes in each. The pass moves such a run into a function of its
// own, `F@run<n>`, that ends with `RTS`, and puts `JSR F@run<n>` in each
// place the run was, which keeps each function's code around it as it
// was. A run that ends with a call ends with a `JMP` to the callee
// instead, which returns to the run's caller. `JSR` and `RTS` change no
// register and no flag, so the code after the call sees what it saw
// after the run. The cost is 12 cycles at each place and two bytes of
// the hardware stack while the run executes.
//
// Shared function tails (docs/optimizations/shared_tails.md) share code
// that ends in a `JMP` or `RTS`. This pass takes code in the middle of
// a function, which can't end in one.
//
// Conditions
//
// A run is a sequence of loads, stores, arithmetic and logic
// instructions, compares, `SEC`, `CLC`, `BIT`, and calls, with no label,
// branch, jump, return, push, or pull, no soft-stack operand (`Frame`,
// `Stack`, a compound atom), no name of a function's own `__local_*`
// slot, and no `(zp),Y` pointer in a slot's bytes. That last operand
// carries the pointer's address, which `asm_emit` names by the slot the
// function it's in keeps there. Two functions' slots can share those
// bytes in a unit and be placed apart by the linker, so a run with one,
// moved into the first user's function, would name that function's
// pointer in every place it's called from
// (`asm_aliasing.through_slot_pointer`). The places must not overlap. A
// function with a soft-stack frame is left alone, and so is a run
// inside a loop of at most 64 bytes, where the call's 12 cycles a pass
// would be a large share of the loop's time. In Zork's interpreter that
// rule costs 2 bytes.
//
// A run of `s` bytes at `n` places takes `s + 1 + 3n` bytes in place of
// `n * s`, and the pass makes the change only when that's smaller,
// which for two places means a run of 8 bytes or more. It takes the run
// that saves the most first and repeats.
//
// The new function has no parameters or locals. A call it makes is one
// the functions it came from made, and the link metadata's call graph,
// built from the TAC, already has it as theirs, so the zero-page
// allocation of every function's bytes is unchanged. Its symbol is a
// copy of the first user's, with internal linkage, so it goes in the
// same section.
//
// Python groups the runs by the `repr`s of their instructions. Here
// each instruction's dump stands for its `repr`, since two dumps are
// equal exactly when the `repr`s are.
//
// Where it runs
//
// Last in `compile.optimized_asm`, after `apply_shared_tails`, since a
// tail is cheaper to share with a `JMP` than a run with a `JSR`. Every
// change only removes bytes from a function, so no branch falls out of
// reach.
#include "passes/outline_runs.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "sim/assembler.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

constexpr std::size_t kMaxLen = 24;
// A loop of at most this many bytes keeps its runs in place, since the
// call costs 12 cycles a pass, which in a short loop is a large share.
constexpr long kShortLoop = 64;

bool is_run_kind(const ir::Type_instruction& ins) {
  return isa<ir::Mov, ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor, ir::Inc,
             ir::Dec, ir::ArithmeticShiftLeft, ir::LogicalShiftRight,
             ir::RotateLeft, ir::RotateRight, ir::Compare, ir::BitTest,
             ir::SetCarry, ir::ClearCarry, ir::Call>(ins);
}

bool runnable(const ir::Type_instruction& ins,
              const std::set<asdl::Int>& slot_addrs) {
  if (!is_run_kind(ins)) {
    return false;
  }
  if (through_slot_pointer(ins, slot_addrs)) {
    return false;
  }
  bool ok = true;
  for_each_operand<false>(ins, [&](const ir::Type_operand& op) {
    if (isa<ir::Frame, ir::Stack, ir::Pseudo>(op)) {
      ok = false;
    } else if (const auto* d = as<ir::Data>(op); d && d->name.starts_with("__local_")) {
      ok = false;
    } else if (const auto* x = as<ir::IndexedData>(op);
               x && x->name.starts_with("__local_")) {
      ok = false;
    }
  });
  if (!ok) {
    return false;
  }
  if (const auto* call = as<ir::Call>(ins)) {
    // A call that takes arguments in registers, or keeps them, is left
    // where its registers are set.
    if (!call->reg_args.empty() || !call->preserves.empty()) {
      return false;
    }
  }
  return true;
}

// Returns `(label index, branch index)` for each branch or jump back to
// a label earlier in the function, when the instructions between, the
// loop, take at most `kShortLoop` bytes.
std::vector<std::pair<std::size_t, std::size_t>> loop_spans(
    const Instructions& instrs) {
  const LabelMap at(instrs);
  std::vector<std::pair<std::size_t, std::size_t>> spans;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const std::string* target = nullptr;
    if (const auto* br = as<ir::Branch>(instrs[k])) {
      target = &br->target;
    } else if (const auto* jump = as<ir::Jump>(instrs[k])) {
      target = &jump->target;
    }
    if (target == nullptr) {
      continue;
    }
    const long lab = at.find(*target);
    if (lab < 0 || static_cast<std::size_t>(lab) >= k) {
      continue;
    }
    long size = 0;
    for (std::size_t j = static_cast<std::size_t>(lab); j <= k; ++j) {
      if (!isa<ir::Label>(instrs[j])) {
        size += sim::instruction_size(instrs[j]);
      }
    }
    if (size <= kShortLoop) {
      spans.emplace_back(static_cast<std::size_t>(lab), k);
    }
  }
  return spans;
}

using Place = std::pair<std::size_t, std::size_t>;  // (function index, start)

std::vector<Place> non_overlapping(std::vector<Place> starts, std::size_t length) {
  std::sort(starts.begin(), starts.end());
  std::vector<Place> out;
  std::map<std::size_t, std::size_t> last;
  for (const auto& [fi, k] : starts) {
    const auto it = last.find(fi);
    if (it != last.end() && k < it->second + length) {
      continue;
    }
    out.emplace_back(fi, k);
    last[fi] = k;
  }
  return out;
}

struct Run {
  std::size_t length = 0;
  std::vector<Place> places;
};

// Returns the run that saves the most, or nothing.
std::optional<Run> best_run(const std::vector<ir::Type_top_level>& top,
                            const NameSet* zp_names,
                            const std::set<asdl::Int>& slot_addrs) {
  // Each instruction that can be in a run, in order, with the id of its
  // dump. `ids[fi][k]` is the id at that place, or -1.
  std::vector<Place> keys;
  std::vector<std::vector<long>> ids(top.size());
  std::vector<std::vector<long>> sizes(top.size());
  std::unordered_map<std::string, long> intern;
  std::vector<bool> id_is_call;
  for (std::size_t fi = 0; fi < top.size(); ++fi) {
    const auto* fn = as<ir::Function>(top[fi]);
    if (fn == nullptr) {
      continue;
    }
    const Instructions& instrs = fn->instructions;
    if (std::any_of(instrs.begin(), instrs.end(), [](const auto& i) {
          return isa<ir::FunctionPrologue>(i);
        })) {
      continue;
    }
    ids[fi].assign(instrs.size(), -1);
    sizes[fi].assign(instrs.size(), 0);
    const auto loops = loop_spans(instrs);
    for (std::size_t k = 0; k < instrs.size(); ++k) {
      if (std::any_of(loops.begin(), loops.end(), [&](const auto& span) {
            return span.first <= k && k <= span.second;
          })) {
        continue;
      }
      if (!runnable(instrs[k], slot_addrs)) {
        continue;
      }
      const auto [it, inserted] =
          intern.emplace(asdl::dump(instrs[k]), static_cast<long>(intern.size()));
      if (inserted) {
        id_is_call.push_back(isa<ir::Call>(instrs[k]));
      }
      ids[fi][k] = it->second;
      sizes[fi][k] = sim::instruction_size(instrs[k], zp_names);
      keys.emplace_back(fi, k);
    }
  }
  std::optional<Run> best;
  long best_gain = 0;
  for (std::size_t length = 2; length <= kMaxLen; ++length) {
    // The runs of this length, by their instructions, in the order of
    // their first places.
    std::map<std::vector<long>, std::size_t> group_of;
    std::vector<std::pair<std::vector<long>, std::vector<Place>>> groups;
    for (const auto& [fi, k] : keys) {
      std::vector<long> seq;
      seq.reserve(length);
      bool whole = true;
      for (std::size_t j = k; j < k + length; ++j) {
        if (j >= ids[fi].size() || ids[fi][j] < 0) {
          whole = false;
          break;
        }
        seq.push_back(ids[fi][j]);
      }
      if (!whole) {
        continue;
      }
      const auto [it, inserted] = group_of.emplace(seq, groups.size());
      if (inserted) {
        groups.emplace_back(std::move(seq), std::vector<Place>{});
      }
      groups[it->second].second.emplace_back(fi, k);
    }
    bool any_repeat = false;
    for (const auto& [seq, starts] : groups) {
      auto places = non_overlapping(starts, length);
      const long n = static_cast<long>(places.size());
      if (n < 2) {
        continue;
      }
      any_repeat = true;
      const auto [fi, k] = places[0];
      long s = 0;
      for (std::size_t j = k; j < k + length; ++j) {
        s += sizes[fi][j];
      }
      // The function adds an `RTS`, or none when it ends with a call,
      // which becomes a jump.
      const long ret = id_is_call[static_cast<std::size_t>(seq.back())] ? 0 : 1;
      const long gain = n * s - (s + ret + 3 * n);
      if (gain > best_gain) {
        best_gain = gain;
        best = Run{.length = length, .places = std::move(places)};
      }
    }
    if (!any_repeat) {
      break;
    }
  }
  return best;
}

}  // namespace

asm_ast::Program apply_outline_runs(asm_ast::Program prog,
                                    const NameSet* zp_names,
                                    const std::set<asdl::Int>& slot_addrs,
                                    std::vector<NewFunction>* new_functions) {
  std::vector<ir::Type_top_level>& top = prog.top_level;
  long count = 0;
  while (true) {
    const auto best = best_run(top, zp_names, slot_addrs);
    if (!best) {
      return prog;
    }
    const std::size_t length = best->length;
    const auto [first_fi, first_k] = best->places[0];
    const auto& first_fn = std::get<ir::Function>(top[first_fi]);
    const std::string first_name = first_fn.name;
    const std::string name = first_name + "@run" + std::to_string(count);
    count += 1;
    Instructions body(first_fn.instructions.begin() + static_cast<long>(first_k),
                      first_fn.instructions.begin() +
                          static_cast<long>(first_k + length));
    std::map<std::size_t, std::vector<std::size_t>> by_fn;
    for (const auto& [fi, start] : best->places) {
      by_fn[fi].push_back(start);
    }
    for (auto& [fi, starts] : by_fn) {
      auto& instrs = std::get<ir::Function>(top[fi]).instructions;
      std::sort(starts.rbegin(), starts.rend());
      for (const std::size_t start : starts) {
        const auto first = instrs.begin() + static_cast<long>(start);
        instrs.erase(first + 1, first + static_cast<long>(length));
        *first = ir::Call{.name = name};
      }
    }
    if (new_functions != nullptr) {
      new_functions->push_back(NewFunction{.name = name, .owner = first_name});
    }
    std::size_t at = 0;
    for (std::size_t i = 0; i < top.size(); ++i) {
      const auto* fn = as<ir::Function>(top[i]);
      if (fn != nullptr && fn->name == first_name) {
        at = i;
        break;
      }
    }
    // A run that ends with a call ends with a jump to the callee, which
    // returns to the run's caller.
    Instructions end;
    if (const auto* call = as<ir::Call>(body.back())) {
      const std::string callee = call->name;
      body.pop_back();
      end = std::move(body);
      end.push_back(ir::Jump{.target = callee});
    } else {
      end = std::move(body);
      end.push_back(ir::Return{});
    }
    top.insert(top.begin() + static_cast<long>(at) + 1,
               ir::Function{.name = name, .is_global = false,
                            .instructions = std::move(end)});
  }
}

}  // namespace c6502::passes
