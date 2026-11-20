// Tests the low byte of a value that an INC chain has just
// incremented.
//
// The multi-byte INC peephole (`inc_peephole`) turns an add of 1 to a
// value of 2 or more bytes in memory into
//
//     INC M0
//     BNE done
//     INC M1
//     ...
//   done:
//
// and a test of the low byte right after it, `if ((uint8_t)x == 0)` on
// the value just incremented, follows as
//
//   done:
//     LDA M0
//     BNE T          // or BEQ T
//
// The first `BNE done` is taken exactly when M0 is nonzero, and on the
// other paths to `done` (the fall-through and the `BNE`s after the
// higher bytes) M0 is zero. So the test's outcome is known on every
// path into `done`:
//
// - `BNE T`: the first `BNE done` goes to T instead, and the `BNE T`
//   after the load is dropped, since M0 is zero wherever it's reached.
// - `BEQ T`: the first `BNE done` goes past the `BEQ`, and the `BEQ`
//   becomes a `JMP`.
//
// Each path then skips a load and a branch it used to run. The flags
// where the moved branch arrives are the same, since `INC M0` set N and
// Z from M0's new value, as the load did, and neither touches C or V.
// Only A differs, since the moved branch skips the load, so A must be
// dead where it arrives (`a_dead_at`). M0 is a `Data` or `ZP` byte, the
// load isn't volatile, and `done` is reached only from the chain.
//
// `get_next_code_byte2` in examples/zork/pages.c is
//
//     Z_PC2.lo++;
//     if ((uint8_t)Z_PC2.lo == 0) { ... }
//
// which becomes `INC Z_PC2; BNE .if_end` for each byte read.
//
// The pass runs in the peephole fixed point, right after the INC
// peephole.
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

// Returns the index of the first `Label` named `name`, or nothing.
std::optional<std::size_t> label_index(const Instructions& instrs, const std::string& name) {
  for (std::size_t n = 0; n < instrs.size(); ++n) {
    const auto* label = as<ir::Label>(instrs[n]);
    if (label != nullptr && label->name == name) {
      return n;
    }
  }
  return std::nullopt;
}

// Returns a `.inc_test@N` label that `instrs` doesn't hold yet, with
// the least such N.
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

// Returns the rewritten instructions when an INC chain starts at
// `instrs[i]` and its low byte is tested after its label, or nothing.
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
  // The rest of the chain is an INC, then a `BNE done` or the label.
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
  // Each rewrite replaces the whole list, and the scan goes on from the
  // next index of the new list.
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
