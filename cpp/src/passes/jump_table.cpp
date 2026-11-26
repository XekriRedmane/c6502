#include "passes/jump_table.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/label_numbers.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

constexpr std::size_t kMinCases = 4;
// `RTS` pulls the address the two `PHA`s pushed and adds one.
constexpr long kDispatch = 11;

bool is_a(const ir::Type_operand& op) {
  const auto* reg = as<ir::Reg>(op);
  return reg != nullptr && isa<ir::A>(reg->reg);
}

bool loads_a(const ir::Type_instruction& ins) {
  const auto* mov = as<ir::Mov>(ins);
  return mov != nullptr && is_a(mov->dst) && !isa<ir::Reg>(mov->src);
}

// `ORA #$00` sets Z from A and leaves A as it was.
bool tests_a(const ir::Type_instruction& ins) {
  const auto* ora = as<ir::Or>(ins);
  return ora != nullptr && is_a(ora->dst) &&
         ora->src == ir::Type_operand{ir::Imm{.value = 0}};
}

struct Case {
  asdl::Int key;
  std::string target;
};

std::vector<Case> chain_at(const Instructions& instrs, std::size_t i,
                           std::size_t& length) {
  const std::size_t start = i;
  std::vector<Case> pairs;
  if (0 < i && i < instrs.size()) {
    const auto* br = as<ir::Branch>(instrs[i]);
    if (br != nullptr && isa<ir::EQ>(br->cond) &&
        (loads_a(instrs[i - 1]) || tests_a(instrs[i - 1]))) {
      pairs.push_back(Case{.key = 0, .target = br->target});
      i += 1;
    }
  }
  while (i + 1 < instrs.size()) {
    const auto* cmp = as<ir::Compare>(instrs[i]);
    const auto* br = as<ir::Branch>(instrs[i + 1]);
    const ir::Imm* imm = cmp != nullptr ? as<ir::Imm>(cmp->right) : nullptr;
    if (!(cmp != nullptr && is_a(cmp->left) && imm != nullptr &&
          0 <= imm->value && imm->value <= 127 && br != nullptr &&
          isa<ir::EQ>(br->cond))) {
      break;
    }
    pairs.push_back(Case{.key = imm->value, .target = br->target});
    i += 2;
  }
  length = i - start;
  return pairs;
}

std::optional<Instructions> table_for(const Instructions& instrs, std::size_t i,
                                      std::size_t end,
                                      const std::vector<Case>& pairs,
                                      asdl::Int& counter) {
  std::set<asdl::Int> distinct;
  for (const auto& c : pairs) {
    distinct.insert(c.key);
  }
  if (distinct.size() != pairs.size()) {
    return std::nullopt;
  }
  const LabelMap labels(instrs);
  const ir::Jump* after =
      end < instrs.size() ? as<ir::Jump>(instrs[end]) : nullptr;
  const bool jump_default =
      after != nullptr && labels.contains(after->target) && after->reg_args.empty();
  std::optional<std::string> default_label;
  std::size_t default_at = 0;
  std::size_t rest = 0;
  if (jump_default) {
    default_label = after->target;
    default_at = static_cast<std::size_t>(labels.find(after->target));
    rest = end + 1;
  } else {
    default_at = end;
    rest = end;
  }
  const asdl::Int lo = *distinct.begin();
  const asdl::Int hi = *distinct.rbegin();
  const asdl::Int base = lo <= 2 ? asdl::Int(0) : lo;
  const asdl::Int chain_bytes =
      2 * static_cast<long>(end - i) + (jump_default ? 3 : 0);
  const asdl::Int table_bytes =
      asdl::Int(base != 0 ? 4 : 0) + 4 + kDispatch + 2 * (hi - base + 1);
  if (table_bytes >= chain_bytes) {
    return std::nullopt;
  }
  std::vector<std::size_t> places;
  for (const auto& c : pairs) {
    const long at = labels.find(c.target);
    if (at < 0) {
      return std::nullopt;
    }
    places.push_back(static_cast<std::size_t>(at));
  }
  places.push_back(default_at);
  for (const std::size_t p : places) {
    if (!(a_dead_at(instrs, labels, p) && flags_dead_at(instrs, labels, p) &&
          c_dead_at(instrs, labels, p))) {
      return std::nullopt;
    }
  }
  std::string index;
  if (std::all_of(places.begin(), places.end(), [&](std::size_t p) {
        return y_dead_at(instrs, labels, p);
      })) {
    index = "Y";
  } else if (std::all_of(places.begin(), places.end(), [&](std::size_t p) {
               return x_dead_at(instrs, labels, p);
             })) {
    index = "X";
  } else {
    return std::nullopt;
  }
  std::optional<ir::Label> new_label;
  if (!default_label) {
    default_label = ".jtdef@" + asdl::int_to_string(counter);
    counter += 1;
    new_label = ir::Label{.name = *default_label};
  }
  const std::string table = ".jtab@" + asdl::int_to_string(counter);
  counter += 1;
  std::vector<std::string> targets;
  for (asdl::Int v = base; v <= hi; v += 1) {
    std::string target = *default_label;
    for (const auto& c : pairs) {
      if (c.key == v) {
        target = c.target;
        break;
      }
    }
    targets.push_back(std::move(target));
  }
  Instructions code;
  const ir::Type_operand reg_a = ir::Reg{.reg = ir::A{}};
  if (base != 0) {
    code.push_back(ir::Compare{.left = reg_a, .right = ir::Imm{.value = base}});
    code.push_back(ir::Branch{.cond = ir::CC{}, .target = *default_label});
  }
  code.push_back(ir::Compare{.left = reg_a, .right = ir::Imm{.value = hi + 1}});
  code.push_back(ir::Branch{.cond = ir::CS{}, .target = *default_label});
  code.push_back(ir::JumpTable{
      .table = table, .index = index, .base = base, .targets = targets});
  if (new_label) {
    code.push_back(*new_label);
  }
  bool has_zero = false;
  for (const auto& c : pairs) {
    has_zero = has_zero || c.key == 0;
  }
  if (i > 0 && has_zero && tests_a(instrs[i - 1])) {
    i -= 1;
  }
  Instructions out(instrs.begin(), instrs.begin() + static_cast<long>(i));
  out.insert(out.end(), code.begin(), code.end());
  out.insert(out.end(), instrs.begin() + static_cast<long>(rest), instrs.end());
  return out;
}

void rewrite_function(ir::Function& fn, asdl::Int& counter) {
  Instructions instrs = fn.instructions;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::size_t length = 0;
    const auto pairs = chain_at(instrs, i, length);
    if (pairs.size() < kMinCases) {
      i += 1;
      continue;
    }
    auto replaced = table_for(instrs, i, i + length, pairs, counter);
    if (!replaced) {
      i += length;
      continue;
    }
    instrs = std::move(*replaced);
    changed = true;
    i += 1;
  }
  if (changed) {
    fn.instructions = std::move(instrs);
  }
}

}  // namespace

asm_ast::Program apply_jump_tables(asm_ast::Program prog) {
  asdl::Int counter = next_label_number(prog, {".jtab@", ".jtdef@"});
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, counter);
    }
  }
  return prog;
}

}  // namespace c6502::passes
