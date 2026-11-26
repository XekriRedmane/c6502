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
constexpr long kBranchReach = 120;

using Instructions = std::vector<ir::Type_instruction>;
using LabelSet = std::unordered_set<std::string>;

bool is_control(const ir::Type_instruction& ins) {
  return isa<ir::Label, ir::Branch, ir::Jump, ir::Return, ir::Ret>(ins);
}

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

std::string tail_key(const Instructions& instrs, std::size_t i, std::size_t n) {
  std::string key;
  for (std::size_t k = i; k < i + n; ++k) {
    key += asdl::dump(instrs[k]);
  }
  return key;
}

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

void rewrite_function(ir::Function& fn, long& counter, LabelSet& labels) {
  Instructions& instrs = fn.instructions;
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
