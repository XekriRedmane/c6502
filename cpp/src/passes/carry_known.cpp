#include "passes/carry_known.h"

#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrList = std::vector<ir::Type_instruction>;

// What is known of C before an instruction. `kUnseen` is the state of
// an instruction no path from the function's entry reaches yet.
enum class Carry { kClear, kSet, kUnknown, kUnseen };

struct Edge {
  std::size_t from = 0;
  Carry c = Carry::kUnknown;
};

bool keeps_c(const ir::Type_instruction& instr) {
  return isa<ir::Mov, ir::Inc, ir::Dec, ir::And, ir::Or, ir::Xor, ir::Push, ir::Pop,
             ir::BitTest, ir::Label, ir::Branch, ir::Jump>(instr);
}

Carry after(const ir::Type_instruction& instr, Carry c) {
  if (isa<ir::SetCarry>(instr)) {
    return Carry::kSet;
  }
  if (isa<ir::ClearCarry>(instr)) {
    return Carry::kClear;
  }
  return keeps_c(instr) ? c : Carry::kUnknown;
}

std::vector<std::pair<std::size_t, Carry>> edges(const InstrList& instrs,
                                                 const LabelMap& labels, std::size_t i,
                                                 Carry c) {
  const auto& ins = instrs[i];
  if (isa<ir::Return, ir::Ret>(ins)) {
    return {};
  }
  if (const auto* j = as<ir::Jump>(ins)) {
    const long at = labels.find(j->target);
    if (at < 0) {
      return {};
    }
    return {{static_cast<std::size_t>(at), c}};
  }
  if (const auto* b = as<ir::Branch>(ins)) {
    Carry taken = c;
    Carry fall = c;
    if (isa<ir::CS>(b->cond)) {
      taken = Carry::kSet;
      fall = Carry::kClear;
    } else if (isa<ir::CC>(b->cond)) {
      taken = Carry::kClear;
      fall = Carry::kSet;
    }
    std::vector<std::pair<std::size_t, Carry>> out;
    if (i + 1 < instrs.size()) {
      out.emplace_back(i + 1, fall);
    }
    const long at = labels.find(b->target);
    if (at >= 0) {
      out.emplace_back(static_cast<std::size_t>(at), taken);
    }
    return out;
  }
  if (i + 1 >= instrs.size()) {
    return {};
  }
  return {{i + 1, after(ins, c)}};
}

struct CarryStates {
  std::vector<Carry> before;
  std::vector<std::vector<Edge>> incoming;
};

CarryStates carry_states(const InstrList& instrs) {
  const LabelMap labels(instrs);
  CarryStates s;
  s.before.assign(instrs.size(), Carry::kUnseen);
  s.incoming.resize(instrs.size());
  if (instrs.empty()) {
    return s;
  }
  s.before[0] = Carry::kUnknown;
  std::vector<std::size_t> work = {0};
  while (!work.empty()) {
    const std::size_t i = work.back();
    work.pop_back();
    for (const auto& [j, c] : edges(instrs, labels, i, s.before[i])) {
      const Carry old = s.before[j];
      if (old == Carry::kUnseen) {
        s.before[j] = c;
        work.push_back(j);
      } else if (old != Carry::kUnknown && old != c) {
        s.before[j] = Carry::kUnknown;
        work.push_back(j);
      }
    }
  }
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (s.before[i] == Carry::kUnseen) {
      continue;
    }
    for (const auto& [j, c] : edges(instrs, labels, i, s.before[i])) {
      s.incoming[j].push_back(Edge{i, c});
    }
  }
  return s;
}

// Returns what a `SEC` or `CLC` sets C to, or nothing for any other
// instruction.
std::optional<Carry> setter_value(const ir::Type_instruction& ins) {
  if (isa<ir::SetCarry>(ins)) {
    return Carry::kSet;
  }
  if (isa<ir::ClearCarry>(ins)) {
    return Carry::kClear;
  }
  return std::nullopt;
}

InstrList drop_zero_adds(InstrList instrs) {
  const CarryStates s = carry_states(instrs);
  const LabelMap labels(instrs);
  InstrList out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto& ins = instrs[i];
    const Carry c = s.before[i];
    bool no_op = false;
    if (const auto* x = as<ir::Add>(ins)) {
      no_op = c == Carry::kClear && is_reg_a(x->dst) && x->src == ir::Type_operand(ir::Imm{.value = 0});
    } else if (const auto* y = as<ir::Sub>(ins)) {
      no_op = c == Carry::kSet && is_reg_a(y->dst) && y->src == ir::Type_operand(ir::Imm{.value = 0});
    }
    if (no_op && v_dead_at(instrs, labels, i + 1)) {
      bool flags_ok = flags_dead_at(instrs, labels, i + 1);
      if (!flags_ok && i > 0) {
        const auto* prev = as<ir::Mov>(instrs[i - 1]);
        flags_ok = prev != nullptr && is_reg_a(prev->dst);
      }
      if (flags_ok) {
        continue;
      }
    }
    out.push_back(ins);
  }
  return out;
}

InstrList drop_known(InstrList instrs) {
  const CarryStates s = carry_states(instrs);
  InstrList out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const std::optional<Carry> v = setter_value(instrs[i]);
    if (!v || s.before[i] != *v) {
      out.push_back(std::move(instrs[i]));
    }
  }
  return out;
}

InstrList move_into_loops(InstrList instrs) {
  const CarryStates s = carry_states(instrs);
  std::set<std::size_t> drop;
  std::map<std::size_t, std::size_t> move;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const std::optional<Carry> want = setter_value(instrs[i]);
    if (!want || s.before[i] == Carry::kUnseen) {
      continue;
    }
    std::size_t p = i;
    while (p > 0 && isa<ir::Label>(instrs[p - 1])) {
      --p;
    }
    if (p == i || p == 0) {
      continue;
    }
    if (isa<ir::Jump, ir::Return, ir::Ret>(instrs[p - 1])) {
      continue;
    }
    std::size_t falls = 0;
    std::size_t others = 0;
    bool others_agree = true;
    for (std::size_t k = p; k <= i; ++k) {
      for (const Edge& e : s.incoming[k]) {
        if (p <= e.from && e.from < i) {
          continue;
        }
        if (e.from == p - 1) {
          ++falls;
        } else {
          ++others;
          others_agree = others_agree && e.c == *want;
        }
      }
    }
    if (falls == 1 && others > 0 && others_agree) {
      drop.insert(i);
      move[p] = i;
    }
  }
  if (drop.empty()) {
    return instrs;
  }
  InstrList out;
  out.reserve(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto it = move.find(k); it != move.end()) {
      out.push_back(instrs[it->second]);
    }
    if (!drop.contains(k)) {
      out.push_back(instrs[k]);
    }
  }
  return out;
}

void rewrite_function(ir::Function& fn) {
  InstrList instrs = drop_zero_adds(fn.instructions);
  instrs = drop_known(std::move(instrs));
  instrs = move_into_loops(std::move(instrs));
  const LabelMap labels(instrs);
  InstrList out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (!setter_value(instrs[i]) || !c_dead_at(instrs, labels, i + 1)) {
      out.push_back(instrs[i]);
    }
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_carry_known(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
