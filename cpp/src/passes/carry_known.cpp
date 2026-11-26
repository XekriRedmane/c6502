#include "passes/carry_known.h"

#include <cstddef>
#include <map>
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

constexpr int kUnknown = -1;
constexpr int kUnseen = -2;

struct Edge {
  std::size_t from = 0;
  int c = kUnknown;
};

bool keeps_c(const ir::Type_instruction& instr) {
  return isa<ir::Mov, ir::Inc, ir::Dec, ir::And, ir::Or, ir::Xor, ir::Push, ir::Pop,
             ir::BitTest, ir::Label, ir::Branch, ir::Jump>(instr);
}

int after(const ir::Type_instruction& instr, int c) {
  if (isa<ir::SetCarry>(instr)) {
    return 1;
  }
  if (isa<ir::ClearCarry>(instr)) {
    return 0;
  }
  return keeps_c(instr) ? c : kUnknown;
}

std::vector<std::pair<std::size_t, int>> edges(const InstrList& instrs, const LabelMap& labels,
                                               std::size_t i, int c) {
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
    int taken = c;
    int fall = c;
    if (isa<ir::CS>(b->cond)) {
      taken = 1;
      fall = 0;
    } else if (isa<ir::CC>(b->cond)) {
      taken = 0;
      fall = 1;
    }
    std::vector<std::pair<std::size_t, int>> out;
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
  std::vector<int> before;
  std::vector<std::vector<Edge>> incoming;
};

CarryStates carry_states(const InstrList& instrs) {
  const LabelMap labels(instrs);
  CarryStates s;
  s.before.assign(instrs.size(), kUnseen);
  s.incoming.resize(instrs.size());
  if (instrs.empty()) {
    return s;
  }
  s.before[0] = kUnknown;
  std::vector<std::size_t> work = {0};
  while (!work.empty()) {
    const std::size_t i = work.back();
    work.pop_back();
    for (const auto& [j, c] : edges(instrs, labels, i, s.before[i])) {
      const int old = s.before[j];
      if (old == kUnseen) {
        s.before[j] = c;
        work.push_back(j);
      } else if (old != kUnknown && old != c) {
        s.before[j] = kUnknown;
        work.push_back(j);
      }
    }
  }
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (s.before[i] == kUnseen) {
      continue;
    }
    for (const auto& [j, c] : edges(instrs, labels, i, s.before[i])) {
      s.incoming[j].push_back(Edge{i, c});
    }
  }
  return s;
}

int setter_value(const ir::Type_instruction& ins) {
  if (isa<ir::SetCarry>(ins)) {
    return 1;
  }
  if (isa<ir::ClearCarry>(ins)) {
    return 0;
  }
  return -1;
}

InstrList drop_zero_adds(InstrList instrs) {
  const CarryStates s = carry_states(instrs);
  const LabelMap labels(instrs);
  InstrList out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto& ins = instrs[i];
    const int c = s.before[i];
    bool no_op = false;
    if (const auto* x = as<ir::Add>(ins)) {
      no_op = c == 0 && is_reg_a(x->dst) && x->src == ir::Type_operand(ir::Imm{.value = 0});
    } else if (const auto* y = as<ir::Sub>(ins)) {
      no_op = c == 1 && is_reg_a(y->dst) && y->src == ir::Type_operand(ir::Imm{.value = 0});
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
    const int v = setter_value(instrs[i]);
    if (v < 0 || s.before[i] == kUnseen || s.before[i] != v) {
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
    const int want = setter_value(instrs[i]);
    if (want < 0 || s.before[i] == kUnseen) {
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
          others_agree = others_agree && e.c == want;
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
    if (setter_value(instrs[i]) < 0 || !c_dead_at(instrs, labels, i + 1)) {
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
