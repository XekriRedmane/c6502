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
      while (share_one(*fn, counter)) {
      }
    }
  }
  return prog;
}

}  // namespace c6502::passes
