/* Counts a loop down to 0 when its counter is used only to count.

# What it does

`for (uint8_t i = 0; i < 20; i++) out(0xA0);` counts `i` up and
compares it with 20 on each pass, `INC i; LDA i; CMP #$14; BCC`. Its
counter is read only by its own test and its own step, so the loop can
count the passes left instead, from 20 down to 0, and test that for 0,
which is `DEC i; BNE`, 3 bytes and 2 cycles a pass fewer.

In SSA form the loop is

    Phi(i.2, [(entry, 0uc), (continue, i.3)])
    JumpIfCmp(GreaterOrEqual, i.2, 20uc, .break)
    ...
    Binary(Add, i.2, 1uc, i.3)

and the pass rewrites it, keeping the names, as

    Phi(i.2, [(entry, 20uc), (continue, i.3)])
    JumpIfCmp(Equal, i.2, 0uc, .break)
    ...
    Binary(Subtract, i.2, 1uc, i.3)

so that `i.2` holds `20 - i` where it held `i`.

# When it applies

- `i.2` is a `Phi` of an unsigned byte with two arguments, a constant
  `c0` and a variable `i.3`, and `Binary(Add, i.2, 1, i.3)` is the only
  definition of `i.3`, which only the `Phi` reads.
- `i.2` is read by that `Add` and by one `JumpIfCmp`, the first
  instruction after the `Phi`s of their block `H`, and by nothing else.
- The jump compares `i.2` with a bound `n`, by `>=` or `==` to leave
  the loop when it's taken or by `<` or `!=` to stay in it. The bound
  may be on the left with the comparison mirrored.
- The bound is a constant `c1` with `c0 <= c1`, or `c0` is 0 and the
  bound is a variable that SSA construction renamed (or a parameter)
  defined in a block that strictly dominates `H`, so the loop doesn't
  change it.
- The block the loop stays in after the test, `S`, has `H` as its only
  predecessor, and `S` dominates the block of the `Add`.

Each pass through the `Add` then comes from `H` with the test showing
`i < n`, so `i` goes from `c0` up to at most `n` and never wraps, and
`i >= n`, `i == n`, and `n - i == 0` all say the same thing. `n - i`
starts at `c1 - c0` (or the variable's value) and drops by 1 at each
step, so the test of it for 0 takes the same branch as the test of `i`
on every pass. */
#include "passes/optimization/count_down.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "bit_precise.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;
using framework::Instrs;
using framework::PassContext;
using BK = ir::Kind_binary_operator;

bool unsigned_byte(const std::string& name, const SymbolTable& symbols) {
  const Symbol* sym = symbols.find(name);
  return sym != nullptr && asdl::isa<c99::UChar, c99::Char>(sym->type);
}

std::optional<Int> const_value(const ir::Type_val& v) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    return bit_precise::raw_value(c->const_);
  }
  return std::nullopt;
}

// Returns whether a taken jump of `op`, with the counter on the left,
// leaves the loop, or none for a comparison the pass doesn't take
// (Python's `_LEAVES`).
std::optional<bool> leaves_of(BK op) {
  switch (op) {
    case BK::GreaterOrEqual:
    case BK::Equal:
      return true;
    case BK::LessThan:
    case BK::NotEqual:
      return false;
    default:
      return std::nullopt;
  }
}

// Returns the comparison with the counter on the right, written with it
// on the left, or none (Python's `_MIRROR`).
std::optional<BK> mirror(BK op) {
  switch (op) {
    case BK::LessOrEqual:
      return BK::GreaterOrEqual;
    case BK::Equal:
      return BK::Equal;
    case BK::GreaterThan:
      return BK::LessThan;
    case BK::NotEqual:
      return BK::NotEqual;
    default:
      return std::nullopt;
  }
}

}  // namespace

ir::Function count_loops_down(ir::Function fn, const PassContext& ctx) {
  if (ctx.symbols == nullptr || ctx.ssa_dsts == nullptr) {
    return fn;
  }
  const SymbolTable& symbols = *ctx.symbols;
  Instrs instrs = fn.instructions;
  const tac_cfg::Cfg cfg = tac_cfg::build_cfg(fn);
  const auto idom = tac_cfg::immediate_dominators(cfg);
  std::vector<int> block_of;
  for (const int bid : cfg.block_order) {
    block_of.insert(block_of.end(), cfg.blocks.find(bid)->instructions.size(), bid);
  }
  std::map<std::string, std::size_t, std::less<>> uses;
  std::map<std::string, std::size_t, std::less<>> def_at;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    for (const auto u : uses_in(instrs[k])) {
      ++uses[std::string(u)];
    }
    for (const auto d : defs_in(instrs[k])) {
      def_at.insert_or_assign(std::string(d), k);
    }
  }
  auto use_count = [&](const std::string& name) -> std::size_t {
    const auto it = uses.find(name);
    return it == uses.end() ? 0 : it->second;
  };
  std::map<std::string, int, std::less<>> label_block;
  for (const int b : cfg.block_order) {
    const auto& bi = cfg.blocks.find(b)->instructions;
    if (const auto* l = std::get_if<ir::Label>(&bi[0])) {
      label_block.insert_or_assign(l->name, b);
    }
  }

  auto dominates = [&](int a, int b) {
    while (idom.contains(b)) {
      if (a == b) {
        return true;
      }
      if (idom.at(b) == b) {
        return false;
      }
      b = idom.at(b);
    }
    return false;
  };

  bool changed = false;
  for (std::size_t ip = 0; ip < instrs.size(); ++ip) {
    const auto* phi = std::get_if<ir::Phi>(&instrs[ip]);
    if (phi == nullptr || phi->args.size() != 2 || !std::holds_alternative<ir::Var>(phi->dst)) {
      continue;
    }
    const std::string p = std::get<ir::Var>(phi->dst).name;
    if (!ctx.is_ssa(p) || !unsigned_byte(p, symbols) || use_count(p) != 2) {
      continue;
    }
    std::vector<std::size_t> consts;
    std::vector<std::size_t> steps;
    for (std::size_t n = 0; n < phi->args.size(); ++n) {
      if (const_value(std::get<ir::PhiArg>(phi->args[n]).source)) {
        consts.push_back(n);
      }
      if (std::holds_alternative<ir::Var>(std::get<ir::PhiArg>(phi->args[n]).source)) {
        steps.push_back(n);
      }
    }
    if (consts.size() != 1 || steps.size() != 1) {
      continue;
    }
    const auto& const_source = std::get<ir::PhiArg>(phi->args[consts[0]]).source;
    const auto& const_arg = std::get<ir::Constant>(const_source);
    const Int c0 = *const_value(const_source);
    const std::string s = std::get<ir::Var>(std::get<ir::PhiArg>(phi->args[steps[0]]).source).name;
    const auto s_def = def_at.find(s);
    if (s_def == def_at.end() || use_count(s) != 1) {
      continue;
    }
    const std::size_t ia = s_def->second;
    const auto* add = std::get_if<ir::Binary>(&instrs[ia]);
    if (add == nullptr || !std::holds_alternative<ir::Add>(add->op) ||
        const_value(add->src2) != Int(1) || !(add->src1 == phi->dst)) {
      continue;
    }
    const int h = block_of[ip];
    if (!idom.contains(h)) {
      continue;
    }
    std::size_t ij = ip + 1;
    while (ij < instrs.size() && std::holds_alternative<ir::Phi>(instrs[ij])) {
      ++ij;
    }
    if (ij >= instrs.size() || block_of[ij] != h) {
      continue;
    }
    const auto* jump = std::get_if<ir::JumpIfCmp>(&instrs[ij]);
    if (jump == nullptr) {
      continue;
    }
    const BK jop = ir::kind(jump->op);
    BK op;
    const ir::Type_val* bound = nullptr;
    if (jump->src1 == phi->dst && leaves_of(jop)) {
      op = jop;
      bound = &jump->src2;
    } else if (jump->src2 == phi->dst && mirror(jop)) {
      op = *mirror(jop);
      bound = &jump->src1;
    } else {
      continue;
    }
    ir::Type_val start;
    if (const auto c1 = const_value(*bound)) {
      if (!(0 <= c0 && c0 <= *c1 && *c1 <= 0xFF)) {
        continue;
      }
      start = ir::Constant{.const_ = bit_precise::with_value(const_arg.const_, *c1 - c0)};
    } else {
      const auto* bv = std::get_if<ir::Var>(bound);
      if (c0 != 0 || bv == nullptr) {
        continue;
      }
      const std::string& k = bv->name;
      if (std::find(fn.params.begin(), fn.params.end(), k) != fn.params.end()) {
        // A parameter has one value through the function.
      } else if (ctx.is_ssa(k) && def_at.contains(k)) {
        const int kb = block_of[def_at.find(k)->second];
        if (kb == h || !dominates(kb, h)) {
          continue;
        }
      } else {
        continue;
      }
      if (!unsigned_byte(k, symbols)) {
        continue;
      }
      start = *bound;
    }
    const bool leaves = *leaves_of(op);
    // The block the loop stays in after the test.
    const auto& succs = cfg.blocks.find(h)->successors;
    const auto taken = label_block.find(jump->target);
    if (taken == label_block.end() || succs.size() != 2) {
      continue;
    }
    std::optional<int> fall;
    for (const int b : succs) {
      if (b != taken->second) {
        fall = b;
        break;
      }
    }
    const std::optional<int> stay = leaves ? fall : std::optional<int>(taken->second);
    if (!stay || *stay == h || cfg.blocks.find(*stay)->predecessors != std::vector<int>{h} ||
        !dominates(*stay, block_of[ia])) {
      continue;
    }
    const ir::Type_val zero = ir::Constant{.const_ = bit_precise::with_value(const_arg.const_, 0)};
    ir::Phi new_phi{.dst = phi->dst, .args = phi->args};
    std::get<ir::PhiArg>(new_phi.args[consts[0]]).source = start;
    ir::JumpIfCmp new_jump{.op = leaves ? ir::Type_binary_operator{ir::Equal{}}
                                        : ir::Type_binary_operator{ir::NotEqual{}},
                           .src1 = phi->dst,
                           .src2 = zero,
                           .target = jump->target};
    ir::Binary new_add{.op = ir::Subtract{}, .src1 = add->src1, .src2 = add->src2, .dst = add->dst};
    instrs[ip] = std::move(new_phi);
    instrs[ij] = std::move(new_jump);
    instrs[ia] = std::move(new_add);
    changed = true;
  }
  if (!changed) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(instrs));
}

}  // namespace c6502::passes
