// Drops a compare whose flags an identical compare on the only path
// into it already set.
//
//     Compare(A, M)          CMP M
//     Branch(cond, L)        BCS L
//     ...                    ...         (no fall-through into L)
//     Label(L)             L:
//     Compare(A, M)          CMP M       <- dropped
//
// A branch changes no flags, so the second compare sets every flag as
// the first did. A compare reached by falling through one or more
// branches right after an identical compare goes the same way. L must
// have no other way in. The instruction before the run of labels that
// ends at the compare has to be a `Jump`, `Return`, or `Ret`, exactly
// one `Branch` or `Jump` in the function may target a label in the run,
// and a label whose address is taken (`ImmLabelLow` / `ImmLabelHigh`)
// disqualifies the run.
//
// A compare of a register with 0 sets N and Z from the register's value
// and sets C. When the instruction that wrote the register set N and Z
// from the same value, as a load, a transfer, or an ALU operation on A
// does, and C is dead after the compare, the compare goes too. Stores
// may come between the two, since they leave the flags and the register
// alone, except one lowered with an `LDY #off` setup.
//
// A compare with the byte an add started from is the add's carry. The
// sum `m + x` wraps below `m` exactly when the add carries out, so `CMP
// m` sets C to the opposite of the add's carry, and the branch on C
// takes the opposite condition with the compare gone (`carry_of_add`).
// Only stores from A may come between the add and the compare, none to
// `m`, and N and Z must be dead after the branch on both paths, since
// the add sets them from the sum.
//
// `CMP #$80` sets C to bit 7, which N holds, and `CMP #1` sets C when
// the register isn't 0, which Z tells. So `CMP #$80; BCS L` (`CPX`,
// `CPY`) becomes `BMI L`, and `BCC` `BPL`, when the instruction that set
// the register set N and Z from it. `CMP #1; BCC L` becomes `BEQ L`, and
// `BCS` `BNE`. N, Z, and C must be dead after the branch on both paths.
// `SEC; SBC #1; BCC L` on A becomes `BEQ L` the same way (`BCS`: `BNE`),
// with A and V dead after the branch as well.
//
// In `SEC; SBC #k; BVC L; EOR #$80; L: BMI T`, an A from 0 to 127 less
// k from 0 to 128 is from -128 to 127, so the subtract never overflows.
// V is clear, the `BVC` is always taken, and the `EOR` never runs. N
// after the subtract is then set exactly when A is below k, which is
// when C is clear, so with A and V dead after the branch on both paths
// the sequence becomes `CMP #k; BCC T` (for `BPL`, `BCS`).
#include "passes/redundant_compare.h"

#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_operands.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace redundant_compare {

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};

const std::vector<long>* LabelRefs::get(std::string_view name) const {
  const auto it = refs.find(name);
  return it == refs.end() ? nullptr : &it->second;
}

LabelRefs label_refs(Instrs instrs) {
  LabelRefs out;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const ir::Type_instruction& ins = instrs[i];
    if (const auto* b = as<ir::Branch>(ins)) {
      out.refs[b->target].push_back(static_cast<long>(i));
    } else if (const auto* j = as<ir::Jump>(ins)) {
      out.refs[j->target].push_back(static_cast<long>(i));
    }
    auto visit = [&](const ir::Type_operand& op) {
      if (const auto* lo = as<ir::ImmLabelLow>(op)) {
        out.refs[lo->name].push_back(-1);
      } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
        out.refs[hi->name].push_back(-1);
      }
    };
    for_each_operand<false>(ins, visit);
    if (const auto* d = as<ir::DecimalArith>(ins)) {
      for (const auto& op : d->left) visit(op);
      for (const auto& op : d->right) visit(op);
      for (const auto& op : d->dst) visit(op);
    }
  }
  return out;
}

LabelRefs refs_with_tables(Instrs instrs) {
  LabelRefs out = label_refs(instrs);
  for (const auto& ins : instrs) {
    if (const auto* t = as<ir::JumpTable>(ins)) {
      out.table_targets.insert(t->targets.begin(), t->targets.end());
    }
  }
  return out;
}

namespace {

const ir::Type_operand* nz_dst(const ir::Type_instruction& ins) {
  return std::visit(
      [](const auto& i) -> const ir::Type_operand* {
        using T = std::decay_t<decltype(i)>;
        if constexpr (std::is_same_v<T, ir::Mov> || std::is_same_v<T, ir::Add> ||
                      std::is_same_v<T, ir::Sub> || std::is_same_v<T, ir::And> ||
                      std::is_same_v<T, ir::Or> || std::is_same_v<T, ir::Xor> ||
                      std::is_same_v<T, ir::ArithmeticShiftLeft> ||
                      std::is_same_v<T, ir::LogicalShiftRight> ||
                      std::is_same_v<T, ir::RotateLeft> ||
                      std::is_same_v<T, ir::RotateRight> || std::is_same_v<T, ir::Inc> ||
                      std::is_same_v<T, ir::Dec> || std::is_same_v<T, ir::Pop>) {
          return &i.dst;
        } else {
          return nullptr;
        }
      },
      ins);
}

bool no_fall_through(const ir::Type_instruction& ins) {
  return isa<ir::Jump, ir::Return, ir::Ret>(ins);
}

bool flags_from_walk(Instrs instrs, std::size_t c, const ir::Type_operand& reg,
                     const LabelRefs* refs, std::set<std::string>& seen) {
  const auto& r = std::get<ir::Reg>(reg);
  for (std::size_t k = c; k-- > 0;) {
    const ir::Type_instruction& ins = instrs[k];
    if (const auto* m = as<ir::Mov>(ins)) {
      if (!isa<ir::Reg>(m->dst) && isa<ir::Reg>(m->src) && !sets_up_y(ins)) {
        continue;
      }
    }
    if (refs != nullptr && isa<ir::Branch>(ins)) {
      continue;
    }
    if (refs != nullptr) {
      if (const auto* label = as<ir::Label>(ins)) {
        if (seen.count(label->name) > 0 || refs->table_targets.count(label->name) > 0) {
          return false;
        }
        seen.insert(label->name);
        const std::vector<long>* preds = refs->get(label->name);
        if (preds != nullptr) {
          for (const long p : *preds) {
            if (p < 0) {
              return false;
            }
          }
          for (const long p : *preds) {
            if (!flags_from_walk(instrs, static_cast<std::size_t>(p), reg, refs, seen)) {
              return false;
            }
          }
        }
        if (k == 0) {
          return false;
        }
        const ir::Type_instruction& before = instrs[k - 1];
        if (no_fall_through(before) || isa<ir::JumpTable>(before)) {
          return preds != nullptr && !preds->empty();
        }
        continue;
      }
    }
    const ir::Type_operand* dst = nz_dst(ins);
    if (dst == nullptr || *dst != reg) {
      return false;
    }
    if (const auto* m = as<ir::Mov>(ins)) {
      if ((isa<ir::Reg>(m->src) && m->src == reg) ||
          (!isa<ir::Reg, ir::Imm>(m->src) && sets_up_y(ins) && isa<ir::Y>(r.reg))) {
        return false;
      }
    }
    return true;
  }
  return false;
}

}  // namespace

bool flags_from(Instrs instrs, std::size_t c, const ir::Type_operand& reg,
                const LabelRefs* refs) {
  std::set<std::string> seen;
  return flags_from_walk(instrs, c, reg, refs, seen);
}

}  // namespace redundant_compare

namespace {

using redundant_compare::flags_from;
using redundant_compare::kA;
using redundant_compare::LabelRefs;
using redundant_compare::no_fall_through;

using Rewrites = std::map<std::size_t, std::optional<ir::Type_instruction>>;

bool is_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_carry_cond(const ir::Type_condition& c) { return isa<ir::CC, ir::CS>(c); }

const ir::Type_instruction* compare_before(Instrs instrs, long b) {
  while (b >= 0 && isa<ir::Branch>(instrs[static_cast<std::size_t>(b)])) {
    --b;
  }
  return b >= 0 ? &instrs[static_cast<std::size_t>(b)] : nullptr;
}

std::set<std::size_t> drop_compares_with_zero(Instrs instrs, const LabelMap& labels,
                                              const LabelRefs& refs) {
  std::set<std::size_t> drop;
  for (std::size_t c = 0; c < instrs.size(); ++c) {
    const ir::Type_instruction& ins = instrs[c];
    if (const auto* cmp = as<ir::Compare>(ins)) {
      const auto* imm = as<ir::Imm>(cmp->right);
      if (isa<ir::Reg>(cmp->left) && imm != nullptr && (imm->value & 0xFF) == 0 &&
          flags_from(instrs, c, cmp->left, &refs) && c_dead_at(instrs, labels, c + 1)) {
        drop.insert(c);
      }
      continue;
    }
    if (isa<ir::Or, ir::And>(ins)) {
      const bool is_or = isa<ir::Or>(ins);
      const ir::Type_operand& src = is_or ? std::get<ir::Or>(ins).src : std::get<ir::And>(ins).src;
      const ir::Type_operand& dst = is_or ? std::get<ir::Or>(ins).dst : std::get<ir::And>(ins).dst;
      const auto* imm = as<ir::Imm>(src);
      if (is_a(dst) && imm != nullptr && (imm->value & 0xFF) == (is_or ? 0 : 0xFF) &&
          flags_from(instrs, c, dst, &refs)) {
        drop.insert(c);
      }
      continue;
    }
    if (const auto* x = as<ir::Xor>(ins)) {
      const auto* imm = as<ir::Imm>(x->src2);
      if (is_a(x->dst) && is_a(x->src1) && imm != nullptr && (imm->value & 0xFF) == 0 &&
          flags_from(instrs, c, x->dst, &refs)) {
        drop.insert(c);
      }
    }
  }
  return drop;
}

Rewrites carry_of_add(Instrs instrs, const LabelMap& labels, const AsmContext& ctx) {
  Rewrites out;
  const std::size_t n = instrs.size();
  for (std::size_t i = 0; i < n; ++i) {
    const auto* load = as<ir::Mov>(instrs[i]);
    if (!(load != nullptr && !load->is_volatile && isa<ir::Data, ir::ZP>(load->src) &&
          is_a(load->dst) && i + 4 < n && isa<ir::ClearCarry>(instrs[i + 1]))) {
      continue;
    }
    const auto* add = as<ir::Add>(instrs[i + 2]);
    if (add == nullptr || !is_a(add->dst) || isa<ir::Reg>(add->src)) {
      continue;
    }
    const ir::Type_operand& m = load->src;
    std::size_t c = i + 3;
    while (c < n) {
      const auto* st = as<ir::Mov>(instrs[c]);
      if (!(st != nullptr && is_a(st->src) && !st->is_volatile && !isa<ir::Reg>(st->dst) &&
            !may_alias(st->dst, m, ctx))) {
        break;
      }
      ++c;
    }
    if (!(c + 1 < n)) {
      continue;
    }
    const auto* cmp = as<ir::Compare>(instrs[c]);
    if (cmp == nullptr || !is_a(cmp->left) || cmp->right != m) {
      continue;
    }
    const auto* branch = as<ir::Branch>(instrs[c + 1]);
    if (branch == nullptr || !is_carry_cond(branch->cond)) {
      continue;
    }
    const long target = labels.find(branch->target);
    if (target < 0 || !flags_dead_at(instrs, labels, c + 2) ||
        !flags_dead_at(instrs, labels, static_cast<std::size_t>(target))) {
      continue;
    }
    ir::Type_condition flipped = isa<ir::CC>(branch->cond) ? ir::Type_condition{ir::CS{}}
                                                           : ir::Type_condition{ir::CC{}};
    out[c] = std::nullopt;
    out[c + 1] = ir::Branch{.cond = flipped, .target = branch->target};
  }
  return out;
}

std::optional<ir::Type_condition> branch_for_compare(asdl::Int k, const ir::Type_condition& c) {
  const bool cs = isa<ir::CS>(c);
  if (k == 0x80) {
    return cs ? ir::Type_condition{ir::MI{}} : ir::Type_condition{ir::PL{}};
  }
  if (k == 0x01) {
    return cs ? ir::Type_condition{ir::NE{}} : ir::Type_condition{ir::EQ{}};
  }
  return std::nullopt;
}

Rewrites compares_as_flags(Instrs instrs, const LabelMap& labels, const LabelRefs& refs) {
  Rewrites out;
  if (instrs.empty()) {
    return out;
  }
  for (std::size_t c = 0; c + 1 < instrs.size(); ++c) {
    const ir::Type_instruction& ins = instrs[c];
    const auto* branch = as<ir::Branch>(instrs[c + 1]);
    if (branch == nullptr || !is_carry_cond(branch->cond)) {
      continue;
    }
    const long target = labels.find(branch->target);
    if (target < 0) {
      continue;
    }
    const std::size_t both[2] = {c + 2, static_cast<std::size_t>(target)};
    bool dead = true;
    for (const std::size_t k : both) {
      if (!(flags_dead_at(instrs, labels, k) && c_dead_at(instrs, labels, k))) {
        dead = false;
        break;
      }
    }
    if (!dead) {
      continue;
    }
    std::vector<std::size_t> drop = {c};
    asdl::Int k = 0;
    const ir::Type_operand* reg = nullptr;
    std::size_t at = c;
    const auto* cmp = as<ir::Compare>(ins);
    const auto* sub = as<ir::Sub>(ins);
    if (cmp != nullptr && isa<ir::Reg>(cmp->left) && isa<ir::Imm>(cmp->right)) {
      k = std::get<ir::Imm>(cmp->right).value & 0xFF;
      reg = &cmp->left;
      at = c;
    } else if (sub != nullptr && is_a(sub->dst) && isa<ir::Imm>(sub->src) &&
               (std::get<ir::Imm>(sub->src).value & 0xFF) == 1 &&
               a_dead_at(instrs, labels, both[0]) && v_dead_at(instrs, labels, both[0]) &&
               a_dead_at(instrs, labels, both[1]) && v_dead_at(instrs, labels, both[1])) {
      k = 1;
      reg = &sub->dst;
      if (c >= 1 && isa<ir::SetCarry>(instrs[c - 1])) {
        drop.push_back(c - 1);
        at = c - 1;
      } else if (c >= 2 && isa<ir::SetCarry>(instrs[c - 2]) && isa<ir::Mov>(instrs[c - 1]) &&
                 is_a(std::get<ir::Mov>(instrs[c - 1]).dst)) {
        drop.push_back(c - 2);
        at = c;
      } else {
        continue;
      }
    } else {
      continue;
    }
    const auto new_cond = branch_for_compare(k, branch->cond);
    if (!new_cond || !flags_from(instrs, at, *reg, &refs)) {
      continue;
    }
    bool taken = out.count(c + 1) > 0;
    for (const std::size_t j : drop) {
      taken = taken || out.count(j) > 0;
    }
    if (taken) {
      continue;
    }
    for (const std::size_t j : drop) {
      out[j] = std::nullopt;
    }
    out[c + 1] = ir::Branch{.cond = *new_cond, .target = branch->target};
  }
  return out;
}

bool a_nonnegative_at(Instrs instrs, std::size_t s, const LabelRefs& refs) {
  std::size_t k = s;
  std::vector<std::string> names;
  while (k > 0 && isa<ir::Label>(instrs[k - 1])) {
    --k;
    names.push_back(std::get<ir::Label>(instrs[k]).name);
  }
  if (names.empty() || k == 0) {
    return false;
  }
  for (const auto& name : names) {
    if (refs.table_targets.count(name) > 0) {
      return false;
    }
    if (const std::vector<long>* preds = refs.get(name)) {
      for (const long p : *preds) {
        const ir::Branch* br =
            p >= 0 ? as<ir::Branch>(instrs[static_cast<std::size_t>(p)]) : nullptr;
        if (!(br != nullptr && isa<ir::PL>(br->cond) &&
              flags_from(instrs, static_cast<std::size_t>(p), kA, &refs))) {
          return false;
        }
      }
    }
  }
  const ir::Type_instruction& before = instrs[k - 1];
  if (no_fall_through(before) || isa<ir::JumpTable>(before)) {
    return true;
  }
  const auto* br = as<ir::Branch>(before);
  return br != nullptr && isa<ir::MI>(br->cond) && flags_from(instrs, k - 1, kA, &refs);
}

Rewrites signed_compare_of_nonnegative(Instrs instrs, const LabelMap& labels,
                                       const LabelRefs& refs) {
  Rewrites out;
  for (std::size_t s = 0; s + 5 < instrs.size(); ++s) {
    const auto* sub = as<ir::Sub>(instrs[s + 1]);
    const auto* bvc = as<ir::Branch>(instrs[s + 2]);
    const auto* eor = as<ir::Xor>(instrs[s + 3]);
    const auto* lab = as<ir::Label>(instrs[s + 4]);
    const auto* br = as<ir::Branch>(instrs[s + 5]);
    if (!(isa<ir::SetCarry>(instrs[s]) && sub != nullptr && is_a(sub->dst) &&
          isa<ir::Imm>(sub->src))) {
      continue;
    }
    const asdl::Int value = std::get<ir::Imm>(sub->src).value;
    if (!(0 <= value && value <= 0x80 && bvc != nullptr && isa<ir::VC>(bvc->cond) &&
          eor != nullptr && is_a(eor->dst) && is_a(eor->src1) &&
          eor->src2 == ir::Type_operand{ir::Imm{.value = 0x80}} && lab != nullptr &&
          bvc->target == lab->name)) {
      continue;
    }
    const std::vector<long>* lab_refs = refs.get(lab->name);
    if (!(lab_refs != nullptr && *lab_refs == std::vector<long>{static_cast<long>(s + 2)} &&
          br != nullptr && isa<ir::MI, ir::PL>(br->cond) && a_nonnegative_at(instrs, s, refs))) {
      continue;
    }
    out[s + 2] = std::nullopt;
    out[s + 3] = std::nullopt;
    out[s + 4] = std::nullopt;
    const long target = labels.find(br->target);
    if (target >= 0 && a_dead_at(instrs, labels, s + 6) && v_dead_at(instrs, labels, s + 6) &&
        a_dead_at(instrs, labels, static_cast<std::size_t>(target)) &&
        v_dead_at(instrs, labels, static_cast<std::size_t>(target))) {
      out[s] = std::nullopt;
      out[s + 1] = ir::Compare{.left = kA, .right = sub->src};
      out[s + 5] = ir::Branch{.cond = isa<ir::MI>(br->cond) ? ir::Type_condition{ir::CC{}}
                                                            : ir::Type_condition{ir::CS{}},
                              .target = br->target};
    }
    return out;
  }
  return out;
}

void apply_rewrites(std::vector<ir::Type_instruction>& instrs, const Rewrites& rewrites) {
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto it = rewrites.find(i);
    if (it == rewrites.end()) {
      out.push_back(std::move(instrs[i]));
    } else if (it->second) {
      out.push_back(*it->second);
    }
  }
  instrs = std::move(out);
}

void drop_indexes(std::vector<ir::Type_instruction>& instrs, const std::set<std::size_t>& drop) {
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (drop.count(i) == 0) {
      out.push_back(std::move(instrs[i]));
    }
  }
  instrs = std::move(out);
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx) {
  const Instrs instrs = fn.instructions;
  const LabelMap labels(instrs);
  const LabelRefs refs = redundant_compare::refs_with_tables(instrs);
  Rewrites carry = carry_of_add(instrs, labels, ctx);
  if (carry.empty()) {
    carry = compares_as_flags(instrs, labels, refs);
  }
  if (carry.empty()) {
    carry = signed_compare_of_nonnegative(instrs, labels, refs);
  }
  if (!carry.empty()) {
    apply_rewrites(fn.instructions, carry);
    return;
  }
  std::set<std::size_t> drop = drop_compares_with_zero(instrs, labels, refs);
  if (!drop.empty()) {
    drop_indexes(fn.instructions, drop);
    return;
  }
  for (std::size_t c = 0; c < instrs.size(); ++c) {
    const auto* cmp = as<ir::Compare>(instrs[c]);
    if (cmp == nullptr) {
      continue;
    }
    if (c > 0 && isa<ir::Branch>(instrs[c - 1])) {
      const ir::Type_instruction* before = compare_before(instrs, static_cast<long>(c) - 1);
      if (before != nullptr && *before == instrs[c]) {
        drop.insert(c);
      }
      continue;
    }
    std::size_t s = c;
    while (s > 0 && isa<ir::Label>(instrs[s - 1])) {
      --s;
    }
    if (s == c || s == 0) {
      continue;
    }
    if (!no_fall_through(instrs[s - 1])) {
      continue;
    }
    std::vector<long> sources;
    for (std::size_t j = s; j < c; ++j) {
      if (const std::vector<long>* r = refs.get(std::get<ir::Label>(instrs[j]).name)) {
        sources.insert(sources.end(), r->begin(), r->end());
      }
    }
    if (sources.size() != 1) {
      continue;
    }
    const long b = sources[0];
    if (b < 1 || !isa<ir::Branch>(instrs[static_cast<std::size_t>(b)])) {
      continue;
    }
    const ir::Type_instruction* before = compare_before(instrs, b);
    if (before == nullptr || *before != instrs[c]) {
      continue;
    }
    drop.insert(c);
  }
  if (!drop.empty()) {
    drop_indexes(fn.instructions, drop);
  }
}

}  // namespace

asm_ast::Program apply_redundant_compare(asm_ast::Program prog, const AsmContext& ctx) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx);
    }
  }
  return prog;
}

}  // namespace c6502::passes
