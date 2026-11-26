#include "passes/inc_low_byte_test.h"

#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

constexpr std::string_view kTestPrefix = ".inc_test@";

const ir::Branch* branch_ne(const Instructions& instrs, std::size_t k) {
  if (k >= instrs.size()) {
    return nullptr;
  }
  const auto* br = as<ir::Branch>(instrs[k]);
  return br != nullptr && isa<ir::NE>(br->cond) ? br : nullptr;
}

std::optional<std::size_t> label_index(const Instructions& instrs, const std::string& name) {
  for (std::size_t n = 0; n < instrs.size(); ++n) {
    const auto* label = as<ir::Label>(instrs[n]);
    if (label != nullptr && label->name == name) {
      return n;
    }
  }
  return std::nullopt;
}

std::string fresh_label(const Instructions& instrs) {
  std::set<long long> used;
  for (const auto& ins : instrs) {
    const auto* label = as<ir::Label>(ins);
    if (label == nullptr || !label->name.starts_with(kTestPrefix)) {
      continue;
    }
    const std::string_view digits = std::string_view(label->name).substr(kTestPrefix.size());
    if (digits.empty()) {
      continue;
    }
    long long n = 0;
    bool ok = true;
    for (char c : digits) {
      if (c < '0' || c > '9') {
        ok = false;
        break;
      }
      n = n * 10 + (c - '0');
    }
    if (ok) {
      used.insert(n);
    }
  }
  long long n = 0;
  while (used.contains(n)) {
    ++n;
  }
  return std::string(kTestPrefix) + std::to_string(n);
}

// `INC M` sets N and Z from M's new value, as `LDA M` does, and neither
// touches C or V.
std::optional<Instructions> try_rewrite(const Instructions& instrs, std::size_t i) {
  const auto* first = as<ir::Inc>(instrs[i]);
  if (first == nullptr || !isa<ir::Data, ir::ZP>(first->dst)) {
    return std::nullopt;
  }
  const ir::Type_operand& m0 = first->dst;
  const ir::Branch* br = branch_ne(instrs, i + 1);
  if (br == nullptr) {
    return std::nullopt;
  }
  const std::string done = br->target;
  std::set<std::size_t> chain_branches{i + 1};
  std::size_t k = i + 2;
  while (true) {
    if (!(k < instrs.size() && isa<ir::Inc>(instrs[k]))) {
      return std::nullopt;
    }
    ++k;
    const ir::Branch* next = branch_ne(instrs, k);
    if (next != nullptr && next->target == done) {
      chain_branches.insert(k);
      ++k;
      continue;
    }
    break;
  }
  const ir::Label* done_label = k < instrs.size() ? as<ir::Label>(instrs[k]) : nullptr;
  if (done_label == nullptr || done_label->name != done) {
    return std::nullopt;
  }
  const std::size_t label = k;
  for (std::size_t n = 0; n < instrs.size(); ++n) {
    const std::string* target = nullptr;
    if (const auto* b = as<ir::Branch>(instrs[n])) {
      target = &b->target;
    } else if (const auto* j = as<ir::Jump>(instrs[n])) {
      target = &j->target;
    }
    if (target != nullptr && *target == done && !chain_branches.contains(n)) {
      return std::nullopt;
    }
  }
  if (label + 2 >= instrs.size()) {
    return std::nullopt;
  }
  const auto* lda = as<ir::Mov>(instrs[label + 1]);
  const auto* test = as<ir::Branch>(instrs[label + 2]);
  if (lda == nullptr || lda->is_volatile) {
    return std::nullopt;
  }
  const auto* lda_dst = as<ir::Reg>(lda->dst);
  if (lda_dst == nullptr || !isa<ir::A>(lda_dst->reg) || !(lda->src == m0) ||
      test == nullptr || !isa<ir::NE, ir::EQ>(test->cond)) {
    return std::nullopt;
  }
  const std::string test_target = test->target;
  Instructions out = instrs;
  if (isa<ir::NE>(test->cond)) {
    const std::optional<std::size_t> target = label_index(instrs, test_target);
    if (!target || !a_dead_at(instrs, *target)) {
      return std::nullopt;
    }
    out[i + 1] = ir::Branch{.cond = ir::NE{}, .target = test_target};
    out.erase(out.begin() + static_cast<std::ptrdiff_t>(label + 2));
  } else {
    if (!a_dead_at(instrs, label + 3)) {
      return std::nullopt;
    }
    const std::string after = fresh_label(instrs);
    out[i + 1] = ir::Branch{.cond = ir::NE{}, .target = after};
    out[label + 2] = ir::Jump{.target = test_target};
    out.insert(out.begin() + static_cast<std::ptrdiff_t>(label + 3), ir::Label{.name = after});
  }
  if (chain_branches.size() == 1) {
    out.erase(out.begin() + static_cast<std::ptrdiff_t>(label));
  }
  return out;
}

void rewrite_function(ir::Function& fn) {
  for (std::size_t i = 0; i < fn.instructions.size(); ++i) {
    if (std::optional<Instructions> rewritten = try_rewrite(fn.instructions, i)) {
      fn.instructions = std::move(*rewritten);
    }
  }
}

}  // namespace

asm_ast::Program apply_inc_low_byte_test(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
