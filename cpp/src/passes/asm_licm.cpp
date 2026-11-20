// Asm-level loop-invariant store hoisting (LICM-lite).
//
// The pass finds three shapes inside a natural loop and hoists each to
// the loop's preheader when it is loop-invariant.
//
//   1. `Mov(Imm, Data | ZP)`, a single constant store.
//   2. `Mov(Imm, Reg(A)); Mov(Reg(A), Data | ZP)`, the form of (1)
//      before fusion.
//   3. `Mov(Data | ZP, Reg(A)); Mov(Reg(A), Data | ZP)`, a read of a
//      stable memory cell written into another stable memory cell. The
//      DPTR staging for a volatile pointer dereference (`LDA static_ptr;
//      STA DPTR; LDA static_ptr+1; STA DPTR+1`) inside an outer loop is
//      the motivating case. The static pointer doesn't change between
//      iterations, so the staging belongs in the preheader.
//
// The motivating case for shapes (1) and (2) is a loop-invariant
// zp_abi argument write (`LDA #$01; STA __zpabi_callee_p0` inside a
// loop that calls `callee` with a constant width argument). Without
// LICM the constant is stored again on every iteration.
//
// What counts as a natural loop
//
// A Jump or Branch whose target Label appears earlier in the
// instruction stream is a back edge. The loop's body is the contiguous
// range `[header_idx, back_edge_idx]`, inclusive, where `header_idx` is
// the index of the back edge's target Label and `back_edge_idx` the
// index of the Jump or Branch.
//
// A function may hold several back edges, and each is processed on its
// own. Nested loops are handled by running the pass to a fixed point,
// since a hoist out of an inner loop may move an instruction to where
// the outer loop can hoist it further.
//
// Eligibility
//
// A candidate inside a loop body is hoisted when all of these are true.
//
//   * Nothing else in the loop body writes the dst (`Data` or `ZP`).
//     A store through a pointer, or with an index register, that
//     `may_alias` can't separate from the dst counts as a write too.
//   * For shape (3), the read-then-write pair, nothing in the loop body
//     writes the src (`Data` or `ZP`) either. Otherwise the loaded value
//     could change between iterations.
//   * No `Call` is in the loop body. This keeps the pass from reasoning
//     about whether a callee might write the dst. A call to a `zp_abi`
//     callee writes its `__zpabi_<callee>_p<k>` slots, and the callee
//     may change them further as locals, so the slot can't be taken to
//     still hold the constant on the next iteration.
//   * No instruction outside the loop body branches into the body past
//     the header, so the header is the single entry. The check is that
//     the header Label is the only label in `[header_idx,
//     back_edge_idx]` that a branch or jump outside the range names.
//   * Every read of the dst inside the loop comes after the store on
//     every path from the header, so no read in the first iteration
//     sees the dst's value from before the loop. A pointer staged in
//     DPTR and read by `STA (DPTR),Y` later in the same iteration
//     passes. A header that reads a loop variable the body resets at
//     its end (`for (p = &x; p != 0; ) { ...; p = 0; }`) fails.
//   * The dst is dead at every exit from the loop that can be reached
//     before the store has run. Otherwise a loop that exits before
//     reaching the store, such as a `for` loop that runs zero times,
//     would leave the hoisted value where the old value used to be.
//   * A, N, and Z are dead at the header, where the hoisted `LDA` now
//     runs, and dead just after the candidate, which no longer sets
//     them. A load or a store leaves C and V alone.
//   * For shape (3), the copy isn't DPTR staging from a pointer that is
//     already in zero page (a name in `zp_cells` below `$100`). Memory
//     value propagation turns `(DPTR),Y` after such a staging into
//     `(ptr),Y`, which needs no staging at all, but only while the
//     staging is in the loop with the access.
//   * Neither half of a pair, nor the single-instruction form, carries
//     `is_volatile`. A volatile Mov stays at its place in the source
//     order so the access happens on every iteration, and hoisting one
//     out of the loop would change the number of accesses.
//
// Hoist mechanics
//
// The eligible Mov (or the `LDA #c; STA M` pair) is removed from the
// body and inserted right before `header_idx`. The preheader is the
// run of instructions that ends just before the Label. A Label with no
// such run, such as one that is the function's first instruction, is
// still a valid insertion point, since the instructions before it
// still flow through it.
//
// Where to run
//
// After `replace_pseudoregisters_bare_exit`, so the operands are
// concrete `Data` / `ZP`, and before the peephole fixed point, so the
// peepholes see the hoisted form. compile.py runs it once, before the
// first peephole fixed point.
#include "passes/asm_licm.h"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrVec = std::vector<ir::Type_instruction>;

// Returns whether a copy from `src` to `dst` stages a byte of a pointer
// that is already in zero page into DPTR.
bool zp_pointer_staging(const ir::Type_operand& src, const ir::Type_operand& dst,
                        const SymbolAddrs& zp_cells) {
  const auto* d = as<ir::Data>(dst);
  if (d == nullptr || d->name != "DPTR") {
    return false;
  }
  if (isa<ir::ZP>(src)) {
    return true;
  }
  const auto* s = as<ir::Data>(src);
  if (s == nullptr) {
    return false;
  }
  const auto it = zp_cells.find(s->name);
  const asdl::Int base = it == zp_cells.end() ? asdl::Int(0x100) : it->second;
  return base + s->offset < 0x100;
}

// Returns the `Data` or `ZP` operand that identifies the cell `op`
// names, or nothing for any other operand. Python keys the cells by
// `(kind, name or address, offset)`, which is the operand itself.
std::optional<ir::Type_operand> operand_key(const ir::Type_operand& op) {
  if (isa<ir::Data, ir::ZP>(op)) {
    return op;
  }
  return std::nullopt;
}

// Whether the instruction struct `T` has a `dst` field that holds one
// operand.
template <class T>
constexpr bool kHasOperandDst = requires(const T& t) {
  requires std::is_same_v<std::remove_cvref_t<decltype(t.dst)>, ir::Type_operand>;
};

// Returns the destination operand of an instruction kind whose `dst`
// is one operand, or null.
const ir::Type_operand* dst_of(const ir::Type_instruction& inst) {
  return std::visit(
      [](const auto& i) -> const ir::Type_operand* {
        if constexpr (kHasOperandDst<std::remove_cvref_t<decltype(i)>>) {
          return &i.dst;
        } else {
          return nullptr;
        }
      },
      inst);
}

// Returns the key of the `Data` / `ZP` cell that `inst` writes, or
// nothing when `inst` writes no such cell.
std::optional<ir::Type_operand> written_data_key(const ir::Type_instruction& inst) {
  switch (ir::kind(inst)) {
    case ir::Kind_instruction::Mov:
    case ir::Kind_instruction::Add:
    case ir::Kind_instruction::Sub:
    case ir::Kind_instruction::And:
    case ir::Kind_instruction::Or:
    case ir::Kind_instruction::Inc:
    case ir::Kind_instruction::Dec:
    case ir::Kind_instruction::Xor:
    case ir::Kind_instruction::ArithmeticShiftLeft:
    case ir::Kind_instruction::LogicalShiftRight:
    case ir::Kind_instruction::RotateLeft:
    case ir::Kind_instruction::RotateRight:
    case ir::Kind_instruction::LoadAddress:
      return operand_key(*dst_of(inst));
    default:
      return std::nullopt;
  }
}

// Returns whether `inst` stores through a pointer or with an index
// register to a location that may be `cell`. Direct `Data` / `ZP`
// writes are counted exactly by `written_data_key`.
bool may_write(const ir::Type_instruction& inst, const ir::Type_operand& cell,
               const AsmContext& ctx) {
  switch (ir::kind(inst)) {
    case ir::Kind_instruction::Mov:
    case ir::Kind_instruction::Add:
    case ir::Kind_instruction::Sub:
    case ir::Kind_instruction::And:
    case ir::Kind_instruction::Or:
    case ir::Kind_instruction::Xor:
    case ir::Kind_instruction::Inc:
    case ir::Kind_instruction::Dec:
    case ir::Kind_instruction::ArithmeticShiftLeft:
    case ir::Kind_instruction::LogicalShiftRight:
    case ir::Kind_instruction::RotateLeft:
    case ir::Kind_instruction::RotateRight:
    case ir::Kind_instruction::LoadAddress:
    case ir::Kind_instruction::Pop:
      break;
    default:
      return false;
  }
  const ir::Type_operand& dst = *dst_of(inst);
  if (isa<ir::Reg, ir::Data, ir::ZP>(dst)) {
    return false;
  }
  return may_alias(dst, cell, ctx);
}

// A hoistable store found at an index: the number of instructions it
// spans, the key of the cell it writes, and for shape (3) the key of
// the cell it reads, which must not be written in the loop.
struct Candidate {
  std::size_t span_len;
  ir::Type_operand dst_key;
  std::optional<ir::Type_operand> src_key;
};

// Returns the hoistable store at `instrs[start]`, or nothing. The three
// shapes are the ones in the comment at the top of the file. Shape (1)
// is one instruction, and shapes (2) and (3) are a pair that moves
// together: the first loads A from an immediate or a stable cell, and
// the second writes A to a stable cell. A volatile Mov in either place
// disqualifies the candidate, since the access has to stay where it is
// in the source order.
std::optional<Candidate> match_candidate(Instrs instrs, std::size_t start,
                                         std::size_t last) {
  const auto* inst = as<ir::Mov>(instrs[start]);
  if (inst == nullptr || inst->is_volatile) {
    return std::nullopt;
  }
  // Shape 1: Mov(Imm, Data|ZP) directly.
  if (isa<ir::Imm>(inst->src)) {
    if (auto key = operand_key(inst->dst)) {
      return Candidate{1, *key, std::nullopt};
    }
  }
  if (is_reg_a(inst->dst) && start + 1 <= last) {
    const auto* nxt = as<ir::Mov>(instrs[start + 1]);
    if (nxt != nullptr && !nxt->is_volatile && is_reg_a(nxt->src)) {
      if (auto dst_key = operand_key(nxt->dst)) {
        // Shape 2: Mov(Imm, A); Mov(A, Data|ZP). An immediate is always
        // invariant.
        if (isa<ir::Imm>(inst->src)) {
          return Candidate{2, *dst_key, std::nullopt};
        }
        // Shape 3: Mov(Data|ZP, A); Mov(A, Data|ZP). The caller checks
        // that the loop doesn't write the source.
        if (auto src_key = operand_key(inst->src)) {
          return Candidate{2, *dst_key, *src_key};
        }
      }
    }
  }
  return std::nullopt;
}

// Returns the indices that can run right after `instrs[p]`.
std::vector<std::size_t> successors_of(Instrs instrs, std::size_t p,
                                       const LabelMap& labels) {
  const auto& inst = instrs[p];
  if (isa<ir::Ret, ir::Return>(inst)) {
    return {};
  }
  if (const auto* j = as<ir::Jump>(inst)) {
    const long t = labels.find(j->target);
    if (t >= 0) {
      return {static_cast<std::size_t>(t)};
    }
    return {};
  }
  std::vector<std::size_t> out;
  if (p + 1 < instrs.size()) {
    out.push_back(p + 1);
  }
  if (const auto* b = as<ir::Branch>(inst)) {
    const long t = labels.find(b->target);
    if (t >= 0) {
      out.push_back(static_cast<std::size_t>(t));
    }
  }
  return out;
}

// Returns whether moving the store `instrs[k .. k + span_len)` to just
// before the header changes no value that anything reads: not the dst
// inside the loop or after it, and not A or the flags around either
// position.
bool hoist_preserves_values(Instrs instrs, const LabelMap& labels,
                            std::size_t header_idx, std::size_t back_edge_idx,
                            std::size_t k, std::size_t span_len,
                            const std::string& fn_name, const AsmContext& ctx) {
  const std::size_t last = k + span_len - 1;
  const ir::Type_operand& dst = *dst_of(instrs[last]);
  if (!(a_dead_at(instrs, labels, header_idx) &&
        flags_dead_at(instrs, labels, header_idx) &&
        a_dead_at(instrs, labels, last + 1) &&
        flags_dead_at(instrs, labels, last + 1))) {
    return false;
  }
  const auto in_body = [&](std::size_t q) {
    return q >= header_idx && q <= back_edge_idx;
  };
  // done[p - header_idx]: on every path from the header to instrs[p]
  // within one iteration, the store has run. The header's own entry is
  // false, since the first iteration comes from the preheader.
  std::vector<char> done(back_edge_idx - header_idx + 1, 1);
  done[0] = 0;
  bool changed = true;
  while (changed) {
    changed = false;
    for (std::size_t p = header_idx; p <= back_edge_idx; ++p) {
      const bool out = p == last ? true : done[p - header_idx] != 0;
      for (std::size_t q : successors_of(instrs, p, labels)) {
        if (in_body(q) && q != header_idx && done[q - header_idx] && !out) {
          done[q - header_idx] = 0;
          changed = true;
        }
      }
    }
  }
  for (std::size_t p = header_idx; p <= back_edge_idx; ++p) {
    if (k <= p && p <= last) {
      continue;
    }
    const bool done_p = done[p - header_idx] != 0;
    const bool out = p == last ? true : done_p;
    if (!done_p && asm_dead_store::reads(instrs[p], dst, ctx)) {
      return false;
    }
    if (isa<ir::Ret, ir::Return>(instrs[p])) {
      if (!done_p) {
        return false;
      }
      continue;
    }
    if (out) {
      continue;
    }
    for (std::size_t q : successors_of(instrs, p, labels)) {
      if (in_body(q)) {
        continue;
      }
      // `mem_dead_after(q)` starts after `instrs[q]`, which skips
      // nothing only when `instrs[q]` is a Label.
      if (!isa<ir::Label>(instrs[q])) {
        return false;
      }
      if (!asm_dead_store::mem_dead_after(instrs, labels, q, dst, ctx, fn_name)) {
        return false;
      }
    }
  }
  return true;
}

// Tries to find one hoistable Mov in `[header_idx, back_edge_idx]`.
// Returns the instructions with the hoist applied, or nothing when no
// hoist is possible.
std::optional<InstrVec> try_hoist_one(const InstrVec& instrs, const LabelMap& labels,
                                      std::size_t header_idx,
                                      std::size_t back_edge_idx,
                                      const std::string& header_name,
                                      const std::string& fn_name,
                                      const SymbolAddrs& zp_cells,
                                      const AsmContext& ctx) {
  // Single-entry check: every branch or jump into the body must target
  // `header_name`. A label inside the body that something outside the
  // body names would be a side entry.
  if (!asm_licm::is_single_entry(instrs, header_idx, back_edge_idx, header_name)) {
    return std::nullopt;
  }
  for (std::size_t k = header_idx; k <= back_edge_idx; ++k) {
    // No Calls in the body, which keeps the pass conservative. A packed
    // BCD chain writes several bytes, which the single-dst write count
    // below doesn't model.
    if (isa<ir::Call, ir::DecimalArith>(instrs[k])) {
      return std::nullopt;
    }
  }
  // The number of writes of each Data / ZP cell inside the body, to
  // recognize "only this Mov writes M" (for the dst check) and "no
  // instruction writes this cell" (for the src check of the
  // read-then-write shape).
  std::vector<std::pair<ir::Type_operand, int>> write_count;
  const auto count_of = [&](const ir::Type_operand& key) {
    for (const auto& [k, n] : write_count) {
      if (k == key) {
        return n;
      }
    }
    return 0;
  };
  for (std::size_t k = header_idx; k <= back_edge_idx; ++k) {
    if (auto key = written_data_key(instrs[k])) {
      bool found = false;
      for (auto& [kk, n] : write_count) {
        if (kk == *key) {
          ++n;
          found = true;
          break;
        }
      }
      if (!found) {
        write_count.emplace_back(*key, 1);
      }
    }
  }
  for (std::size_t k = header_idx; k <= back_edge_idx; ++k) {
    const auto candidate = match_candidate(instrs, k, back_edge_idx);
    if (!candidate) {
      continue;
    }
    const std::size_t span_len = candidate->span_len;
    if (count_of(candidate->dst_key) != 1) {
      continue;
    }
    // For the read-then-write shape, the src cell must not be written
    // anywhere in the body. Otherwise the loaded value isn't
    // loop-invariant.
    if (candidate->src_key && count_of(*candidate->src_key) != 0) {
      continue;
    }
    const ir::Type_operand& last_dst = *dst_of(instrs[k + span_len - 1]);
    const ir::Type_operand& first_src = std::get<ir::Mov>(instrs[k]).src;
    if (candidate->src_key && zp_pointer_staging(first_src, last_dst, zp_cells)) {
      continue;
    }
    // A store through a pointer or with an index register can write the
    // same cell under another name. `g = 5; out = g; *p = 7;` in a loop
    // with `p == &g` must keep `g = 5` inside.
    std::vector<ir::Type_operand> cells{last_dst};
    if (candidate->src_key) {
      cells.push_back(first_src);
    }
    bool written = false;
    for (std::size_t j = header_idx; j <= back_edge_idx && !written; ++j) {
      if (j >= k && j < k + span_len) {
        continue;
      }
      for (const auto& cell : cells) {
        if (may_write(instrs[j], cell, ctx)) {
          written = true;
          break;
        }
      }
    }
    if (written) {
      continue;
    }
    if (!hoist_preserves_values(instrs, labels, header_idx, back_edge_idx, k,
                                span_len, fn_name, ctx)) {
      continue;
    }
    // Hoist. The candidate occupies [k, k + span_len), which moves to
    // the position right before `header_idx`.
    InstrVec out;
    out.reserve(instrs.size());
    out.insert(out.end(), instrs.begin(),
               instrs.begin() + static_cast<std::ptrdiff_t>(header_idx));
    out.insert(out.end(), instrs.begin() + static_cast<std::ptrdiff_t>(k),
               instrs.begin() + static_cast<std::ptrdiff_t>(k + span_len));
    out.insert(out.end(), instrs.begin() + static_cast<std::ptrdiff_t>(header_idx),
               instrs.begin() + static_cast<std::ptrdiff_t>(k));
    out.insert(out.end(), instrs.begin() + static_cast<std::ptrdiff_t>(k + span_len),
               instrs.end());
    return out;
  }
  return std::nullopt;
}

// Finds one hoistable Mov in the innermost loop that has one and
// performs the hoist. Returns the new instructions, or nothing when no
// hoist fires.
std::optional<InstrVec> one_pass(const InstrVec& instrs, const std::string& fn_name,
                                 const SymbolAddrs& zp_cells, const AsmContext& ctx) {
  const LabelMap labels(instrs);
  // The back edges, as (header_idx, back_edge_idx, header_name).
  std::vector<std::tuple<std::size_t, std::size_t, std::string>> back_edges;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto target = asm_licm::branch_or_jump_target(instrs[k]);
    if (!target) {
      continue;
    }
    const long header_idx = labels.find(*target);
    if (header_idx < 0) {
      continue;
    }
    if (static_cast<std::size_t>(header_idx) < k) {
      back_edges.emplace_back(static_cast<std::size_t>(header_idx), k, *target);
    }
  }
  // Inner loops (the smallest ranges) first, so a hoist out of an inner
  // loop can also leave any enclosing loop on the next iteration.
  std::stable_sort(back_edges.begin(), back_edges.end(),
                   [](const auto& a, const auto& b) {
                     return std::get<1>(a) - std::get<0>(a) <
                            std::get<1>(b) - std::get<0>(b);
                   });
  for (const auto& [header_idx, back_edge_idx, header_name] : back_edges) {
    if (auto hoisted = try_hoist_one(instrs, labels, header_idx, back_edge_idx,
                                     header_name, fn_name, zp_cells, ctx)) {
      return hoisted;
    }
  }
  return std::nullopt;
}

}  // namespace

namespace asm_licm {

std::optional<std::string> branch_or_jump_target(const ir::Type_instruction& inst) {
  if (const auto* j = as<ir::Jump>(inst)) {
    return j->target;
  }
  if (const auto* b = as<ir::Branch>(inst)) {
    return b->target;
  }
  return std::nullopt;
}

bool is_single_entry(Instrs instrs, std::size_t begin, std::size_t end,
                     const std::string& header_name) {
  std::set<std::string, std::less<>> body_label_names;
  for (std::size_t k = begin; k <= end; ++k) {
    if (const auto* label = as<ir::Label>(instrs[k])) {
      body_label_names.insert(label->name);
    }
  }
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (k >= begin && k <= end) {
      continue;
    }
    const auto target = branch_or_jump_target(instrs[k]);
    if (!target) {
      continue;
    }
    if (body_label_names.contains(*target) && *target != header_name) {
      return false;
    }
  }
  return true;
}

}  // namespace asm_licm

asm_ast::Program apply_licm(asm_ast::Program prog, const AsmContext& ctx,
                            const SymbolAddrs* zp_cells) {
  static const SymbolAddrs kEmpty;
  const SymbolAddrs& cells = zp_cells != nullptr ? *zp_cells : kEmpty;
  for (auto& tl : prog.top_level) {
    auto* fn = as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    while (auto next = one_pass(fn->instructions, fn->name, cells, ctx)) {
      fn->instructions = std::move(*next);
    }
  }
  return prog;
}

}  // namespace c6502::passes
