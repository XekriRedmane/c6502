// Merges repeated return tails into one copy that branches reach.
//
// A function with several early returns of the same value ends each
// one with the same few instructions, such as `LDX #0; TXA; RTS` for
// `return 0` from a function returning a 16-bit value in A and X. When
// a copy is skipped over by a conditional branch,
//
//     Branch(cond, L)          BCS L
//     <tail>                   LDX #0 / TXA / RTS
//     Label(L)               L:
//
// it becomes a branch on the opposite condition to another copy of the
// same tail:
//
//     Branch(!cond, T)         BCC T
//     Label(L)               L:
//
// That saves the tail's instructions at each site. The copy that's kept
// is the last one in the function. If it isn't already labeled, a fresh
// label `.tail@N` goes in front of it, which changes nothing for the
// code that falls into it.
//
// A tail is a run of 2 to `kMaxTail` instructions ending in a `Return`,
// containing no labels, branches, or jumps, so it behaves the same
// wherever it runs. Hand-written 6502 code shares a not-found exit the
// same way. A lone `RTS` isn't merged, since the branch to it would
// save at most one instruction.
//
// A branch or jump to a label in front of a tail goes to the kept copy
// instead, when that's another copy. The label then has one reference
// fewer, and once it has none, `dead_label_drop` removes it, so a
// branch over the tail becomes a site. `three_way_compare` leaves two
// branches to the `return 0` of a sorted-table lookup, one that skips
// over it and one to its label, and both go to the one copy this way.
//
// A site is rewritten only when the new branch's estimated distance,
// from the byte sizes `expand_long_branches` uses, is within
// `kBranchReach` bytes. Otherwise `expand_long_branches` would turn it
// into an inverted branch over a `JMP`, which is no shorter than the
// tail it replaced.
//
// The Python pass keys the tails by the `repr` of their instructions,
// since asm nodes aren't hashable. Here they are keyed by their dumps,
// which are equal exactly when the instructions are.
#include "passes/tail_merge.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/long_branches.h"
#include "sim/assembler.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

constexpr std::size_t kMaxTail = 4;
// A margin under the 127 bytes a branch reaches, since later passes can
// still move code between the branch and its target.
constexpr long kBranchReach = 120;

using Instructions = std::vector<ir::Type_instruction>;
using LabelSet = std::unordered_set<std::string>;

bool is_control(const ir::Type_instruction& ins) {
  return isa<ir::Label, ir::Branch, ir::Jump, ir::Return, ir::Ret>(ins);
}

// Returns the length of the tail starting at `instrs[i]`, or nothing
// when no run of plain instructions ending in a `Return` starts there.
std::optional<std::size_t> tail_at(const Instructions& instrs, std::size_t i) {
  for (std::size_t n = 2; n <= kMaxTail; ++n) {
    if (i + n > instrs.size()) {
      return std::nullopt;
    }
    const auto& last = instrs[i + n - 1];
    if (isa<ir::Return>(last)) {
      for (std::size_t k = i; k < i + n - 1; ++k) {
        if (is_control(instrs[k])) {
          return std::nullopt;
        }
      }
      return n;
    }
    if (is_control(last)) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

// Returns the key of the tail of `n` instructions at `instrs[i]`.
std::string tail_key(const Instructions& instrs, std::size_t i, std::size_t n) {
  std::string key;
  for (std::size_t k = i; k < i + n; ++k) {
    key += asdl::dump(instrs[k]);
  }
  return key;
}

// Returns whether a branch at index `b` to the instruction at `keep` is
// estimated to be within `kBranchReach` bytes. An instruction whose
// size can't be computed makes the answer false.
bool in_reach(const Instructions& instrs, std::size_t b, std::size_t keep) {
  const std::size_t lo = std::min(b + 1, keep);
  const std::size_t hi = std::max(b + 1, keep);
  long dist = 0;
  try {
    for (std::size_t k = lo; k < hi; ++k) {
      if (!isa<ir::Label>(instrs[k])) {
        dist += sim::instruction_size(instrs[k]);
      }
    }
  } catch (const std::exception&) {
    return false;
  }
  return dist <= kBranchReach;
}

// Returns a label `.tail@N` that isn't in `labels`, from N = `counter`
// up, adds it to `labels`, and leaves `counter` one past the N used.
std::string fresh_label(long& counter, LabelSet& labels) {
  std::string name = ".tail@" + std::to_string(counter);
  while (labels.contains(name)) {
    counter += 1;
    name = ".tail@" + std::to_string(counter);
  }
  labels.insert(name);
  counter += 1;
  return name;
}

// Sends each branch or jump to a label in front of a tail to the last
// copy of that tail, when that's another copy.
void retarget_function(ir::Function& fn, long& counter, LabelSet& labels) {
  Instructions& instrs = fn.instructions;
  std::unordered_map<std::string, std::size_t> last_copy;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (const auto n = tail_at(instrs, i)) {
      last_copy[tail_key(instrs, i, *n)] = i;
    }
  }
  if (last_copy.empty()) {
    return;
  }
  const LabelMap position(instrs);
  std::map<std::size_t, std::string> insert_label;
  std::map<std::size_t, ir::Type_instruction> rewrite;
  for (std::size_t b = 0; b < instrs.size(); ++b) {
    const auto& ins = instrs[b];
    const auto* branch = as<ir::Branch>(ins);
    const auto* jump = as<ir::Jump>(ins);
    if (branch == nullptr && jump == nullptr) {
      continue;
    }
    const long at = position.find(branch ? branch->target : jump->target);
    if (at < 0) {
      continue;
    }
    std::size_t start = static_cast<std::size_t>(at);
    while (start < instrs.size() && isa<ir::Label>(instrs[start])) {
      start += 1;
    }
    const auto n = tail_at(instrs, start);
    if (!n) {
      continue;
    }
    const std::size_t keep = last_copy.at(tail_key(instrs, start, *n));
    if (keep == start) {
      continue;
    }
    if (branch != nullptr && !in_reach(instrs, b, keep)) {
      continue;
    }
    std::string target;
    const ir::Label* prev = keep > 0 ? as<ir::Label>(instrs[keep - 1]) : nullptr;
    if (prev != nullptr) {
      target = prev->name;
    } else if (const auto it = insert_label.find(keep); it != insert_label.end()) {
      target = it->second;
    } else {
      target = fresh_label(counter, labels);
      insert_label[keep] = target;
    }
    if (branch != nullptr) {
      rewrite[b] = ir::Branch{.cond = branch->cond, .target = target};
    } else {
      rewrite[b] = ir::Jump{.target = target};
    }
  }
  if (rewrite.empty()) {
    return;
  }
  Instructions out;
  out.reserve(instrs.size() + insert_label.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (const auto it = insert_label.find(i); it != insert_label.end()) {
      out.push_back(ir::Label{.name = it->second});
    }
    if (auto it = rewrite.find(i); it != rewrite.end()) {
      out.push_back(std::move(it->second));
    } else {
      out.push_back(std::move(instrs[i]));
    }
  }
  instrs = std::move(out);
}

// Rewrites the sites in one function. `labels` holds every label in the
// program and gains the ones made here.
void rewrite_function(ir::Function& fn, long& counter, LabelSet& labels) {
  Instructions& instrs = fn.instructions;
  // The sites, `Branch(c, L); <tail>; Label(L)` at index b, with the
  // tail's length.
  std::vector<std::pair<std::size_t, std::size_t>> sites;
  for (std::size_t b = 0; b < instrs.size(); ++b) {
    const auto* branch = as<ir::Branch>(instrs[b]);
    if (branch == nullptr) {
      continue;
    }
    const auto n = tail_at(instrs, b + 1);
    if (!n) {
      continue;
    }
    const std::size_t after = b + 1 + *n;
    if (!(after < instrs.size() && isa<ir::Label>(instrs[after]) &&
          std::get<ir::Label>(instrs[after]).name == branch->target)) {
      continue;
    }
    sites.emplace_back(b, *n);
  }
  if (sites.empty()) {
    return;
  }
  // The kept copy of each tail is its last occurrence outside every
  // site's tail (a site's own tail may be kept, at its start), since a
  // site's tail is what gets removed.
  std::vector<bool> inside(instrs.size(), false);
  for (const auto& [b, n] : sites) {
    for (std::size_t k = b + 2; k < b + 1 + n; ++k) {
      inside[k] = true;
    }
  }
  std::unordered_map<std::string, std::size_t> last_copy;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (inside[i]) {
      continue;
    }
    if (const auto n = tail_at(instrs, i)) {
      last_copy[tail_key(instrs, i, *n)] = i;
    }
  }
  std::map<std::size_t, std::string> insert_label;
  std::unordered_map<std::string, std::string> target_of;
  std::map<std::size_t, ir::Branch> rewrite;
  std::map<std::size_t, std::size_t> site_length;
  for (const auto& [b, n] : sites) {
    site_length[b] = n;
    const std::string key = tail_key(instrs, b + 1, n);
    const auto kept = last_copy.find(key);
    if (kept == last_copy.end()) {
      continue;
    }
    const std::size_t keep = kept->second;
    if (keep == b + 1) {
      continue;
    }
    if (!target_of.contains(key)) {
      const ir::Label* prev =
          keep > 0 ? as<ir::Label>(instrs[keep - 1]) : nullptr;
      if (prev != nullptr) {
        target_of[key] = prev->name;
      } else {
        // The label is made, and the counter moves on, even when no
        // site ends up branching to it, as in Python.
        const std::string name = fresh_label(counter, labels);
        insert_label[keep] = name;
        target_of[key] = name;
      }
    }
    if (!in_reach(instrs, b, keep)) {
      continue;
    }
    rewrite[b] = ir::Branch{
        .cond = long_branches::invert(std::get<ir::Branch>(instrs[b]).cond),
        .target = target_of[key]};
  }
  if (rewrite.empty()) {
    return;
  }
  std::vector<bool> skip(instrs.size(), false);
  for (const auto& [b, branch] : rewrite) {
    for (std::size_t k = b + 1; k < b + 1 + site_length[b]; ++k) {
      skip[k] = true;
    }
  }
  Instructions out;
  out.reserve(instrs.size() + insert_label.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (const auto it = insert_label.find(i); it != insert_label.end()) {
      out.push_back(ir::Label{.name = it->second});
    }
    if (skip[i]) {
      continue;
    }
    if (auto it = rewrite.find(i); it != rewrite.end()) {
      out.push_back(std::move(it->second));
    } else {
      out.push_back(std::move(instrs[i]));
    }
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_tail_merge(asm_ast::Program prog) {
  // Every label in the program, so that a new `.tail@N` is unique even
  // across functions (the sim's assembler keeps one label table) and
  // across the fixed point's repeated runs of this pass.
  LabelSet labels;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      for (const auto& ins : fn->instructions) {
        if (const auto* label = as<ir::Label>(ins)) {
          labels.insert(label->name);
        }
      }
    }
  }
  long counter = 0;
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      retarget_function(*fn, counter, labels);
      rewrite_function(*fn, counter, labels);
    }
  }
  return prog;
}

}  // namespace c6502::passes
