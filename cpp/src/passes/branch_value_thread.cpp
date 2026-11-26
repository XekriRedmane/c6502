// The labels it adds are numbered across the program, since the sim's
// assembler rejects one name in two functions.
#include "passes/branch_value_thread.h"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"
#include "passes/label_numbers.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

constexpr int kSteps = 24;

bool is_memory(const ir::Type_operand& op) { return isa<ir::Data, ir::ZP>(op); }

bool is_a(const ir::Type_operand& op) {
  const auto* reg = as<ir::Reg>(op);
  return reg != nullptr && isa<ir::A>(reg->reg);
}

bool is_reg(const ir::Type_operand& op) { return isa<ir::Reg>(op); }

bool readable(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::Data, ir::ZP>(op);
}

struct Fact {
  bool eq = false;
  asdl::Int value = 0;
  bool operator==(const Fact&) const = default;
};

struct ByteFacts {
  ir::Type_operand op;
  std::vector<Fact> facts;
};

struct Nz {
  bool cmp = false;
  ir::Type_operand op;
  asdl::Int k = 0;
  bool operator==(const Nz&) const = default;
};

struct State {
  std::optional<ir::Type_operand> a;
  std::optional<Nz> nz;
  std::vector<ByteFacts> facts;

  void forget(const ir::Type_operand& written, const AsmContext& ctx) {
    std::erase_if(facts, [&](const ByteFacts& f) {
      return may_alias(f.op, written, ctx);
    });
    if (a && is_memory(*a) && may_alias(*a, written, ctx)) {
      a.reset();
    }
    if (nz && is_memory(nz->op) && may_alias(nz->op, written, ctx)) {
      nz.reset();
    }
  }

  void learn(const ir::Type_operand& op, Fact fact) {
    if (!is_memory(op)) {
      return;
    }
    ByteFacts* entry = find(op);
    if (entry == nullptr) {
      facts.push_back(ByteFacts{.op = op});
      entry = &facts.back();
    }
    for (const Fact& f : entry->facts) {
      if (f == fact) {
        return;
      }
    }
    entry->facts.push_back(fact);
  }

  ByteFacts* find(const ir::Type_operand& op) {
    for (auto& f : facts) {
      if (f.op == op) {
        return &f;
      }
    }
    return nullptr;
  }
  const ByteFacts* find(const ir::Type_operand& op) const {
    return const_cast<State*>(this)->find(op);
  }

  std::optional<asdl::Int> value_of(const ir::Type_operand& op) const {
    if (const auto* imm = as<ir::Imm>(op)) {
      return imm->value;
    }
    if (is_memory(op)) {
      if (const ByteFacts* entry = find(op)) {
        for (const Fact& f : entry->facts) {
          if (f.eq) {
            return f.value;
          }
        }
      }
    }
    return std::nullopt;
  }

  std::optional<bool> differs_from(const ir::Type_operand& op, asdl::Int v) const {
    if (const auto known = value_of(op)) {
      return (*known & 0xFF) != (v & 0xFF);
    }
    if (is_memory(op)) {
      if (const ByteFacts* entry = find(op)) {
        for (const Fact& f : entry->facts) {
          if (!f.eq && f.value == v) {
            return true;
          }
        }
      }
    }
    return std::nullopt;
  }
};

std::optional<bool> decide(const State& state, const ir::Type_condition& cond) {
  if (!state.nz) {
    return std::nullopt;
  }
  const Nz& nz = *state.nz;
  const ir::Type_operand& op = nz.op;
  const asdl::Int k = nz.cmp ? nz.k : asdl::Int(0);
  if (isa<ir::EQ, ir::NE>(cond)) {
    const auto differs = state.differs_from(op, k);
    if (!differs) {
      return std::nullopt;
    }
    return isa<ir::NE>(cond) ? *differs : !*differs;
  }
  const auto known = state.value_of(op);
  if (!known) {
    return std::nullopt;
  }
  const asdl::Int v = *known & 0xFF;
  if (isa<ir::MI, ir::PL>(cond)) {
    const asdl::Int r = (v - k) & 0xFF;
    const bool neg = (r & 0x80) != 0;
    return isa<ir::MI>(cond) ? neg : !neg;
  }
  if (nz.cmp && isa<ir::CC, ir::CS>(cond)) {
    const bool carry = v >= (k & 0xFF);
    return isa<ir::CS>(cond) ? carry : !carry;
  }
  return std::nullopt;
}

void learn_branch(State& state, const ir::Type_condition& cond, bool taken) {
  if (!state.nz || !isa<ir::EQ, ir::NE>(cond)) {
    return;
  }
  const ir::Type_operand op = state.nz->op;
  const asdl::Int k = state.nz->cmp ? state.nz->k : asdl::Int(0);
  const bool equal = isa<ir::EQ>(cond) == taken;
  state.learn(op, Fact{.eq = equal, .value = k & 0xFF});
}

bool scan_step(State& state, const ir::Type_instruction& ins,
               const AsmContext& ctx) {
  if (const auto* mov = as<ir::Mov>(ins)) {
    const auto& src = mov->src;
    const auto& dst = mov->dst;
    if (is_a(dst)) {
      if (readable(src) && !mov->is_volatile) {
        state.a = src;
        state.nz = Nz{.cmp = false, .op = src};
      } else {
        state.a.reset();
        state.nz.reset();
      }
      return true;
    }
    if (is_reg(dst)) {
      // LDX / LDY / TAX / TAY set N and Z from the new value.
      state.nz.reset();
      return true;
    }
    if (is_memory(dst)) {
      state.forget(dst, ctx);
      if (!is_reg(src)) {
        if (readable(src) && !mov->is_volatile) {
          state.a = src;
          state.nz = Nz{.cmp = false, .op = src};
        } else {
          state.a.reset();
          state.nz.reset();
        }
      }
      return true;
    }
    return false;
  }
  if (const auto* cmp = as<ir::Compare>(ins)) {
    const auto* imm = as<ir::Imm>(cmp->right);
    if (is_a(cmp->left) && imm != nullptr && state.a && readable(*state.a)) {
      state.nz = Nz{.cmp = true, .op = *state.a, .k = imm->value};
    } else {
      state.nz.reset();
    }
    return true;
  }
  if (isa<ir::SetCarry, ir::ClearCarry>(ins)) {
    return true;
  }
  const ir::Type_operand* rmw_dst = std::visit(
      [](const auto& x) -> const ir::Type_operand* {
        using T = std::remove_cvref_t<decltype(x)>;
        if constexpr (std::is_same_v<T, ir::Inc> || std::is_same_v<T, ir::Dec> ||
                      std::is_same_v<T, ir::ArithmeticShiftLeft> ||
                      std::is_same_v<T, ir::LogicalShiftRight> ||
                      std::is_same_v<T, ir::RotateLeft> ||
                      std::is_same_v<T, ir::RotateRight>) {
          return &x.dst;
        } else {
          return nullptr;
        }
      },
      ins);
  if (rmw_dst != nullptr) {
    if (is_memory(*rmw_dst)) {
      state.forget(*rmw_dst, ctx);
    } else if (is_a(*rmw_dst)) {
      state.a.reset();
    } else if (!is_reg(*rmw_dst)) {
      // An element indexed by X (`DEC t,X`) may be any byte of `t` and
      // past it, as a store to one is.
      return false;
    }
    state.nz.reset();
    return true;
  }
  if (isa<ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor>(ins)) {
    state.a.reset();
    state.nz.reset();
    return true;
  }
  return false;
}

struct Changed {
  bool a = false;
  bool nz = false;
  bool c = false;
};

bool equivalent(Instrs instrs, const LabelMap& labels, std::size_t i,
                const State& start, const State& now, const Changed& changed) {
  if (changed.a) {
    const bool same = start.a && now.a && *start.a == *now.a;
    if (!(same || a_dead_at(instrs, labels, i))) {
      return false;
    }
  }
  if (changed.nz) {
    const bool same = start.nz && now.nz && *start.nz == *now.nz;
    if (!(same || flags_dead_at(instrs, labels, i))) {
      return false;
    }
  }
  if (changed.c && !c_dead_at(instrs, labels, i)) {
    return false;
  }
  return true;
}

std::optional<std::size_t> new_target(Instrs instrs, const LabelMap& labels,
                                      const State& start, std::size_t at) {
  State state = start;
  Changed changed;
  bool executed = false;
  std::optional<std::size_t> best;
  std::size_t i = at;
  std::vector<bool> seen(instrs.size(), false);
  for (int step = 0; step < kSteps; ++step) {
    if (i >= instrs.size() || seen[i]) {
      break;
    }
    seen[i] = true;
    const auto& ins = instrs[i];
    if (isa<ir::Label>(ins)) {
      i += 1;
      continue;
    }
    if (executed && equivalent(instrs, labels, i, start, state, changed)) {
      best = i;
    }
    executed = true;
    if (const auto* mov = as<ir::Mov>(ins);
        mov != nullptr && is_a(mov->dst) && readable(mov->src) &&
        !mov->is_volatile) {
      state.a = mov->src;
      state.nz = Nz{.cmp = false, .op = mov->src};
      changed.a = changed.nz = true;
      i += 1;
      continue;
    }
    if (const auto* cmp = as<ir::Compare>(ins);
        cmp != nullptr && is_a(cmp->left) && isa<ir::Imm>(cmp->right) &&
        state.a && readable(*state.a)) {
      state.nz = Nz{.cmp = true, .op = *state.a,
                    .k = std::get<ir::Imm>(cmp->right).value};
      changed.nz = changed.c = true;
      i += 1;
      continue;
    }
    if (const auto* br = as<ir::Branch>(ins)) {
      const auto taken = decide(state, br->cond);
      if (!taken) {
        break;
      }
      learn_branch(state, br->cond, *taken);
      if (*taken) {
        const long j = labels.find(br->target);
        if (j < 0) {
          break;
        }
        i = static_cast<std::size_t>(j);
      } else {
        i += 1;
      }
      continue;
    }
    break;
  }
  return best;
}

void rewrite_function(ir::Function& fn, asdl::Int& counter,
                      const AsmContext& ctx) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::map<std::size_t, std::string> retarget;
  std::map<std::size_t, std::string> added;
  State state;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto& ins = instrs[k];
    if (isa<ir::Label>(ins)) {
      state = State{};
      continue;
    }
    const auto* branch = as<ir::Branch>(ins);
    const auto* jump = as<ir::Jump>(ins);
    if (branch != nullptr || jump != nullptr) {
      const long at = labels.find(branch ? branch->target : jump->target);
      if (at >= 0) {
        std::optional<State> edge = state;
        if (branch != nullptr) {
          const auto taken = decide(*edge, branch->cond);
          if (taken && !*taken) {
            edge.reset();
          } else {
            learn_branch(*edge, branch->cond, true);
          }
        }
        if (edge) {
          const auto dest =
              new_target(instrs, labels, *edge, static_cast<std::size_t>(at));
          if (dest) {
            if (*dest > 0 && isa<ir::Label>(instrs[*dest - 1])) {
              retarget[k] = std::get<ir::Label>(instrs[*dest - 1]).name;
            } else {
              auto it = added.find(*dest);
              if (it == added.end()) {
                it = added
                         .emplace(*dest,
                                  ".vthread@" + asdl::int_to_string(counter))
                         .first;
                counter += 1;
              }
              retarget[k] = it->second;
            }
          }
        }
      }
      if (branch != nullptr) {
        learn_branch(state, branch->cond, false);
        continue;
      }
      state = State{};
      continue;
    }
    if (!scan_step(state, ins, ctx)) {
      state = State{};
    }
  }
  if (retarget.empty()) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size() + added.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto it = added.find(k); it != added.end()) {
      out.push_back(ir::Label{.name = it->second});
    }
    if (const auto it = retarget.find(k); it != retarget.end()) {
      if (auto* br = as<ir::Branch>(instrs[k])) {
        br->target = it->second;
      } else {
        instrs[k] = ir::Jump{.target = it->second};
      }
    }
    out.push_back(std::move(instrs[k]));
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_branch_value_thread(asm_ast::Program prog,
                                           const AsmContext& ctx) {
  asdl::Int counter = next_label_number(prog, {".vthread@"});
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, counter, ctx);
    }
  }
  return prog;
}

}  // namespace c6502::passes
