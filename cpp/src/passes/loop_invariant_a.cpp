// Keeps a loop-invariant compare operand in A across a loop.
//
// Motivating shape
//
// A search loop compares each table entry against a key that doesn't
// change. `tac_to_asm` loads the varying side into A and compares it
// against the key.
//
//     .loop:                         ; loop header
//        LDA   $2200,Y               ; table entry           4 cycles
//        CMP   key                   ; loop-invariant        3 cycles
//        BNE   .next
//        ...                         ; rarer path, clobbers A
//     .next:
//        INY
//        CPY   n
//        BNE   .loop
//
// The hand-written 6502 idiom keeps the key in A for the whole loop and
// compares the entry against it (`CMP $2200,Y`, 4 cycles), loading A
// again only on the paths that overwrite it. Zork I's page-table search
// is written that way.
//
// Rewrite
//
// 1. The compare is swapped. `LDA x; CMP k; Bxx` with Bxx = BEQ / BNE
//    becomes `LDA k; CMP x; Bxx`. Equality is symmetric, so Z is
//    unchanged. N and C do change, and A ends up holding `k` in place
//    of `x`, so the swap requires A and every flag to be dead after the
//    branch on both of its successors.
// 2. The load is hoisted. The `LDA k` at the top of the header is
//    deleted. An `LDA k` goes in front of the loop, on the fall-through
//    entry, and on every edge inside the loop that reaches the header
//    with A no longer equal to `k` (a path that overwrote A). The hot
//    path, which doesn't touch A, gets no load at all.
//
// Eligibility
//
// - The loop is a range `[h, b]` of the instruction list, `Label(H)` at
//   `h` and the last backward branch or jump to H at `b`. Nothing
//   outside the range jumps into it, so it's entered only by falling
//   into `Label(H)`. `c99_to_tac` lays every loop out this way.
// - The header starts `Label(H); Mov(x, A); Compare(A, k);
//   Branch(EQ|NE, _)`, with `k` a `Data` / `ZP` operand and `x` an
//   operand CMP can address.
// - No instruction in the range writes anything that may alias `k`
//   (`may_alias`), and the range has no `Call`.
// - A reload on a taken branch edge P -> S needs a new label in front
//   of S's label. That's only possible when the instruction before S's
//   label never falls through (a `Jump`, `Return`, or `Ret`), so the
//   reload runs only on the redirected edge. Otherwise the pass leaves
//   the loop alone.
// - Blocks between an A-overwriting path and the header must not read
//   or write A, so that a reload on their incoming edge can't change
//   what they compute.
//
// Where it runs
//
// After the final peephole fixed point and before
// `expand_long_branches`, so it sees concrete operands and the layout
// the peepholes settled on, and the stubs it adds get long-branch
// expansion like everything else.
#include "passes/loop_invariant_a.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrList = std::vector<ir::Type_instruction>;
using LabelIndex = std::unordered_map<std::string, std::size_t>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};

bool is_terminator(const ir::Type_instruction& instr) {
  return isa<ir::Jump, ir::Return, ir::Ret>(instr);
}

bool is_cmp_operand(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::Data, ir::ZP, ir::IndexedData, ir::Frame, ir::Stack,
             ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY>(op);
}

const std::string* target(const ir::Type_instruction& instr) {
  if (const auto* j = as<ir::Jump>(instr)) {
    return &j->target;
  }
  if (const auto* b = as<ir::Branch>(instr)) {
    return &b->target;
  }
  return nullptr;
}

const ir::Type_operand* dst_of(const ir::Type_instruction& instr) {
  return std::visit(
      [](const auto& ins) -> const ir::Type_operand* {
        if constexpr (requires { ins.dst; }) {
          if constexpr (std::is_same_v<std::decay_t<decltype(ins.dst)>, ir::Type_operand>) {
            return &ins.dst;
          } else {
            return nullptr;
          }
        } else {
          return nullptr;
        }
      },
      instr);
}

bool writes_a(const ir::Type_instruction& instr) {
  if (isa<ir::Mov, ir::Pop, ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor,
          ir::ArithmeticShiftLeft, ir::LogicalShiftRight, ir::RotateLeft,
          ir::RotateRight>(instr)) {
    return *dst_of(instr) == kA;
  }
  return isa<ir::Call, ir::FunctionPrologue, ir::AllocateStack, ir::DecimalArith>(instr);
}

bool touches_a(const ir::Type_instruction& instr) {
  if (writes_a(instr)) {
    return true;
  }
  if (const auto* mov = as<ir::Mov>(instr)) {
    return mov->src == kA || !isa<ir::Reg>(mov->dst);
  }
  if (const auto* cmp = as<ir::Compare>(instr)) {
    return cmp->left == kA;
  }
  if (const auto* push = as<ir::Push>(instr)) {
    return push->src == kA;
  }
  return isa<ir::LoadAddress>(instr);
}

std::vector<ir::Type_operand> written_memory(const ir::Type_instruction& instr) {
  if (const auto* da = as<ir::DecimalArith>(instr)) {
    return decimal_arith::writes(*da);
  }
  if (isa<ir::Mov, ir::Pop, ir::LoadAddress, ir::Inc, ir::Dec, ir::ArithmeticShiftLeft,
          ir::LogicalShiftRight, ir::RotateLeft, ir::RotateRight>(instr)) {
    const ir::Type_operand& dst = *dst_of(instr);
    if (isa<ir::Reg>(dst)) {
      return {};
    }
    return {dst};
  }
  return {};
}

bool is_c_writer(const ir::Type_instruction& instr) {
  return isa<ir::Compare, ir::SetCarry, ir::ClearCarry, ir::ArithmeticShiftLeft,
             ir::LogicalShiftRight>(instr);
}

bool is_c_reader(const ir::Type_instruction& instr) {
  return isa<ir::Add, ir::Sub, ir::RotateLeft, ir::RotateRight>(instr);
}

std::optional<std::size_t> find_label(const LabelIndex& label_at, const std::string& name) {
  const auto it = label_at.find(name);
  if (it == label_at.end()) {
    return std::nullopt;
  }
  return it->second;
}

bool n_c_dead_at(const InstrList& instrs, std::size_t idx, const LabelIndex& label_at) {
  std::set<std::tuple<std::size_t, bool, bool>> seen;
  std::vector<std::tuple<std::size_t, bool, bool>> stack = {{idx, true, true}};
  while (!stack.empty()) {
    auto [i, n_live, c_live] = stack.back();
    stack.pop_back();
    while (i < instrs.size() && (n_live || c_live)) {
      if (!seen.insert({i, n_live, c_live}).second) {
        break;
      }
      const auto& ins = instrs[i];
      if (const auto* br = as<ir::Branch>(ins)) {
        if (n_live && isa<ir::MI, ir::PL>(br->cond)) {
          return false;
        }
        if (c_live && isa<ir::CC, ir::CS>(br->cond)) {
          return false;
        }
        const auto t = find_label(label_at, br->target);
        if (!t) {
          return false;
        }
        stack.emplace_back(*t, n_live, c_live);
        ++i;
        continue;
      }
      if (const auto* jump = as<ir::Jump>(ins)) {
        const auto t = find_label(label_at, jump->target);
        if (!t) {
          return false;
        }
        i = *t;
        continue;
      }
      if (c_live) {
        if (const auto* r = as<ir::Return>(ins); r != nullptr && r->save_c) {
          return false;
        }
        if (const auto* r = as<ir::Ret>(ins); r != nullptr && r->save_c) {
          return false;
        }
      }
      if (isa<ir::Return, ir::Ret, ir::Call>(ins)) {
        break;
      }
      if (c_live && is_c_reader(ins)) {
        return false;
      }
      if (sets_flags(ins)) {
        n_live = false;
      }
      if (is_c_writer(ins) || is_c_reader(ins)) {
        c_live = false;
      }
      ++i;
    }
  }
  return true;
}

struct Edge {
  std::size_t p;
  std::size_t q;
  bool jump;
  std::size_t at;
};

std::string stub_name(const std::string& fn_name, int counter) {
  return "." + fn_name + "@lia@" + std::to_string(counter);
}

std::optional<InstrList> try_loop(const InstrList& instrs, std::size_t h, std::size_t b,
                                  const LabelIndex& label_at, const std::string& fn_name,
                                  int& counter, const AsmContext& ctx,
                                  const SymbolAddrs* zp_slot_symbols) {
  if (h + 3 > b) {
    return std::nullopt;
  }
  const auto* lda = as<ir::Mov>(instrs[h + 1]);
  const auto* cmp = as<ir::Compare>(instrs[h + 2]);
  const auto* br = as<ir::Branch>(instrs[h + 3]);
  if (!(lda != nullptr && lda->dst == kA && !lda->is_volatile && is_cmp_operand(lda->src) &&
        cmp != nullptr && cmp->left == kA && isa<ir::Data, ir::ZP>(cmp->right) &&
        br != nullptr && isa<ir::EQ, ir::NE>(br->cond))) {
    return std::nullopt;
  }
  const ir::Type_operand k = cmp->right;
  const ir::Type_operand x = lda->src;
  auto in_range = [&](std::size_t i) { return h <= i && i <= b; };

  std::set<std::string, std::less<>> names_in;
  for (std::size_t i = h; i <= b; ++i) {
    if (const auto* l = as<ir::Label>(instrs[i])) {
      names_in.insert(l->name);
    }
  }
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (in_range(i)) {
      continue;
    }
    if (const std::string* t = target(instrs[i]); t != nullptr && names_in.contains(*t)) {
      return std::nullopt;
    }
  }

  const MayAliasOptions alias_options{.zp_slot_symbols = zp_slot_symbols};
  for (std::size_t i = h; i <= b; ++i) {
    const auto& ins = instrs[i];
    if (isa<ir::Call, ir::FunctionPrologue, ir::AllocateStack, ir::Ret>(ins)) {
      return std::nullopt;
    }
    for (const auto& w : written_memory(ins)) {
      if (may_alias(w, k, ctx, alias_options)) {
        return std::nullopt;
      }
    }
  }

  const auto tgt = find_label(label_at, br->target);
  if (!tgt) {
    return std::nullopt;
  }
  const LabelMap labels(instrs);
  if (!a_dead_at(instrs, labels, h + 4) || !a_dead_at(instrs, labels, *tgt)) {
    return std::nullopt;
  }
  if (!n_c_dead_at(instrs, h + 4, label_at) || !n_c_dead_at(instrs, *tgt, label_at)) {
    return std::nullopt;
  }

  std::set<std::size_t> leaders = {h};
  for (std::size_t i = h; i <= b; ++i) {
    const auto& ins = instrs[i];
    if (isa<ir::Label>(ins)) {
      leaders.insert(i);
    }
    if ((isa<ir::Branch>(ins) || is_terminator(ins)) && i + 1 <= b) {
      leaders.insert(i + 1);
    }
  }
  const std::vector<std::size_t> starts(leaders.begin(), leaders.end());
  std::vector<std::pair<std::size_t, std::size_t>> blocks;
  for (std::size_t n = 0; n < starts.size(); ++n) {
    const std::size_t e = n + 1 < starts.size() ? starts[n + 1] - 1 : b;
    blocks.emplace_back(starts[n], e);
  }
  const std::size_t nblocks = blocks.size();
  std::unordered_map<std::string, std::size_t> block_of_label;
  for (std::size_t bi = 0; bi < nblocks; ++bi) {
    if (const auto* l = as<ir::Label>(instrs[blocks[bi].first])) {
      block_of_label[l->name] = bi;
    }
  }

  std::vector<Edge> edges;
  for (std::size_t bi = 0; bi < nblocks; ++bi) {
    const std::size_t e = blocks[bi].second;
    const auto& last = instrs[e];
    if (const std::string* t = target(last)) {
      if (const auto it = block_of_label.find(*t); it != block_of_label.end()) {
        edges.push_back({bi, it->second, true, e});
      }
    }
    if (!is_terminator(last) && bi + 1 < nblocks) {
      edges.push_back({bi, bi + 1, false, e});
    }
  }

  const std::size_t header = 0;
  auto exit_state = [&](std::size_t bi, bool entry) {
    bool state = entry;
    for (std::size_t i = blocks[bi].first; i <= blocks[bi].second; ++i) {
      if (i == h + 1) {
        continue;
      }
      const auto& ins = instrs[i];
      if (writes_a(ins)) {
        const auto* mov = as<ir::Mov>(ins);
        state = mov != nullptr && mov->src == k && mov->dst == kA;
      }
    }
    return state;
  };

  std::vector<std::vector<std::size_t>> preds(nblocks);
  for (const auto& edge : edges) {
    preds[edge.q].push_back(edge.p);
  }

  auto pure = [&](std::size_t bi) {
    for (std::size_t i = blocks[bi].first; i <= blocks[bi].second; ++i) {
      if (!isa<ir::Label>(instrs[i]) && touches_a(instrs[i])) {
        return false;
      }
    }
    return true;
  };

  std::vector<bool> need(nblocks, false);
  need[header] = true;
  for (bool changed = true; changed;) {
    changed = false;
    for (const auto& edge : edges) {
      if (need[edge.q] && !need[edge.p] && edge.p != header && pure(edge.p)) {
        need[edge.p] = true;
        changed = true;
      }
    }
  }

  std::vector<bool> entry_ok(nblocks, true);
  for (bool changed = true; changed;) {
    changed = false;
    for (std::size_t bi = 0; bi < nblocks; ++bi) {
      if (need[bi]) {
        continue;
      }
      bool v = !preds[bi].empty();
      for (const std::size_t p : preds[bi]) {
        if (!exit_state(p, entry_ok[p])) {
          v = false;
          break;
        }
      }
      if (v != entry_ok[bi]) {
        entry_ok[bi] = v;
        changed = true;
      }
    }
  }

  std::vector<Edge> reloads;
  for (const auto& edge : edges) {
    if (!need[edge.q]) {
      continue;
    }
    if (exit_state(edge.p, entry_ok[edge.p])) {
      continue;
    }
    if (edge.q != header && !need[edge.p] && !pure(edge.q)) {
      return std::nullopt;
    }
    reloads.push_back(edge);
  }
  for (const auto& edge : reloads) {
    if (edge.q != header && !pure(edge.q)) {
      return std::nullopt;
    }
  }

  const ir::Type_instruction reload = ir::Mov{.src = k, .dst = kA, .is_volatile = false};
  std::map<std::size_t, InstrList> inserts;
  std::map<std::size_t, std::string> retarget;
  InstrList entry_stub = {reload};
  std::optional<std::string> header_stub_label;
  for (const auto& edge : reloads) {
    const std::size_t s_q = blocks[edge.q].first;
    if (!edge.jump) {
      if (edge.q == header) {
        return std::nullopt;
      }
      inserts[s_q].push_back(reload);
      continue;
    }
    if (edge.q == header) {
      if (!header_stub_label) {
        ++counter;
        header_stub_label = stub_name(fn_name, counter);
        entry_stub = {ir::Label{.name = *header_stub_label}, reload};
      }
      retarget[edge.at] = *header_stub_label;
      continue;
    }
    ++counter;
    const std::string stub_label = stub_name(fn_name, counter);
    if (s_q == 0 || !is_terminator(instrs[s_q - 1])) {
      return std::nullopt;
    }
    if (!isa<ir::Label>(instrs[s_q])) {
      return std::nullopt;
    }
    auto& at = inserts[s_q];
    at.push_back(ir::Label{.name = stub_label});
    at.push_back(reload);
    retarget[edge.at] = stub_label;
  }
  InstrList out;
  out.reserve(instrs.size() + 4);
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (i == h) {
      out.insert(out.end(), entry_stub.begin(), entry_stub.end());
    }
    if (i != h) {
      if (const auto it = inserts.find(i); it != inserts.end()) {
        out.insert(out.end(), it->second.begin(), it->second.end());
      }
    }
    if (i == h + 1) {
      continue;
    }
    if (i == h + 2) {
      out.push_back(ir::Compare{.left = kA, .right = x});
      continue;
    }
    if (const auto it = retarget.find(i); it != retarget.end()) {
      if (const auto* b2 = as<ir::Branch>(instrs[i])) {
        out.push_back(ir::Branch{.cond = b2->cond, .target = it->second});
      } else {
        const auto& j = std::get<ir::Jump>(instrs[i]);
        out.push_back(ir::Jump{.target = it->second, .reg_args = j.reg_args});
      }
      continue;
    }
    out.push_back(instrs[i]);
  }
  return out;
}

std::optional<InstrList> rewrite_one(const InstrList& instrs, const std::string& fn_name,
                                     int& counter, const AsmContext& ctx,
                                     const SymbolAddrs* zp_slot_symbols) {
  LabelIndex label_at;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto* l = as<ir::Label>(instrs[k])) {
      label_at[l->name] = k;
    }
  }
  for (std::size_t h = 0; h < instrs.size(); ++h) {
    const auto* head = as<ir::Label>(instrs[h]);
    if (head == nullptr) {
      continue;
    }
    std::optional<std::size_t> b;
    for (std::size_t k = h + 1; k < instrs.size(); ++k) {
      if (const std::string* t = target(instrs[k]); t != nullptr && *t == head->name) {
        b = k;
      }
    }
    if (!b) {
      continue;
    }
    if (auto result = try_loop(instrs, h, *b, label_at, fn_name, counter, ctx, zp_slot_symbols)) {
      return result;
    }
  }
  return std::nullopt;
}

}  // namespace

asm_ast::Program apply_loop_invariant_a(asm_ast::Program prog, const AsmContext& ctx,
                                        const SymbolAddrs* zp_slot_symbols) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      int counter = 0;
      while (auto next = rewrite_one(fn->instructions, fn->name, counter, ctx, zp_slot_symbols)) {
        fn->instructions = std::move(*next);
      }
    }
  }
  return prog;
}

}  // namespace c6502::passes
