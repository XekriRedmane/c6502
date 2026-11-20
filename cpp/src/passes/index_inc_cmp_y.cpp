// Promotes `mem[X] + 1; CMP n` to `LDY mem,X; INY; CPY n` when Y is
// free and the compare is the only consumer.
//
// Pattern
//
// Five consecutive instructions:
//
//     [i]   Mov(IndexedData(name, index=X), Reg(A))   ; LDA m,X
//     [i+1] Mov(Reg(A), <Data|ZP>)                    ; STA tmp
//     [i+2] Inc(<same Data|ZP>)                       ; INC tmp
//     [i+3] Mov(<same Data|ZP>, Reg(A))               ; LDA tmp
//     [i+4] Compare(Reg(A), <Imm|Data|ZP>)            ; CMP other
//
// become three:
//
//     [i]   Mov(IndexedData(name, index=X), Reg(Y))   ; LDY m,X
//     [i+1] Inc(Reg(Y))                               ; INY
//     [i+2] Compare(Reg(Y), <Imm|Data|ZP>)            ; CPY other
//
// The `-1` form has the same shape, with `Dec` in atom [i+2] and
// `Dec(Reg(Y))` (DEY) in the rewrite. A source indexed by X with the
// result in Y is the only sound combination. `LDY abs,X` exists and
// `LDY abs,Y` doesn't (the 6502 has no `LDY` mode that reads through
// Y), so the rewrite can't be mirrored for a source indexed by Y and a
// result in X.
//
// `shift_in_a` turns `STA tmp; INC tmp; LDA tmp` into `CLC; ADC #1`
// when C and V are dead, which leaves `LDA m,X; CLC; ADC #1; CMP
// other`. `match_in_a` takes that form too.
//
// Soundness
//
// The original's five atoms leave A and tmp holding mem[X] + 1, set
// the flags from (mem[X] + 1) CMP other, and leave X and Y unchanged.
// The rewrite's three atoms leave Y holding mem[X] + 1 and set the
// flags from (mem[X] + 1) CPY other, which are the same flags, since
// CPY n sets N, Z, and C as CMP n does when the two compare the same
// byte values (Y in the rewrite holds what A held in the original). X,
// A, and tmp are unchanged.
//
// That leaves three differences to gate.
//
//   1. A's value differs, since the rewrite leaves A unchanged where
//      the original sets it to mem[X] + 1. This is sound when A is dead
//      after [i+4].
//   2. Y's value differs, since the rewrite sets it to mem[X] + 1 where
//      the original leaves it unchanged. This is sound when Y's value
//      before the pattern is dead.
//   3. tmp's value differs, since the rewrite leaves it unchanged where
//      the original sets it to mem[X] + 1. This is sound when tmp is
//      dead at [i+5], so that no reachable read of tmp comes before the
//      next write of it. Reachable means along every branch and jump,
//      so a read at a loop top that the back edge reaches counts. tmp
//      must also be compiler-owned scratch that is dead at return
//      (`tmp_dead_after`), because a user static keeps its value after
//      the function returns.
//
// Where this hits
//
// The C expression `target = mem[idx] + 1; if (target == other) {
// ... }` lowers (after inc_peephole) into the five-atom shape. The
// register allocator colors `target`'s storage to a zero-page slot
// (`__local_<fn>__<N>`) because Y isn't a candidate color at
// allocation time. Y's only roles in the IR are indirect-Y addressing
// and CPY operands, and it isn't tracked as a general-purpose byte
// register. This peephole finds the shape routed through zero page
// after coloring and substitutes the lowering in Y when the gates
// pass.
//
// The headline case is the target computation of the chase branch in
// `beam_target_tick`, `floor_ceil[floor_idx] + 1` compared to
// `beam_y`.
//
// Where it runs
//
// Inside the asm peephole fixed point, after `replace_pseudoregisters`
// (operands concrete) and after `apply_inc_peephole` (which collapses
// the +1 to `Inc(tmp)`). The collapsers of an `STA; LDA` round trip
// don't apply here, since `STA tmp; INC tmp; LDA tmp` changes tmp
// between the two. It shrinks five atoms to three (4 bytes and 6
// cycles each time), which is monotone and fine in the fixed point.
#include "passes/index_inc_cmp_y.h"

#include <cstddef>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

// What a match gives: the indexed source, whether the step is a
// decrement, and the compare's right operand.
struct Match {
  ir::Type_operand src;
  bool is_dec = false;
  ir::Type_operand other;
};

bool cpy_addressable(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::Data, ir::ZP>(op);
}

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.compare(0, prefix.size(), prefix) == 0;
}

// Returns whether `instr` is a non-volatile `Mov(IndexedData(_, X),
// Reg(A))`, `LDA m,X`.
bool is_load_indexed_by_x(const ir::Type_instruction& instr) {
  const auto* mov = as<ir::Mov>(instr);
  if (mov == nullptr || mov->is_volatile) {
    return false;
  }
  const auto* src = as<ir::IndexedData>(mov->src);
  return src != nullptr && isa<ir::X>(src->index) && is_reg_a(mov->dst);
}

// Returns whether `tmp` is a compiler-owned scratch byte whose value
// after `instrs[idx]` isn't read on any path forward.
//
// Only two kinds of byte qualify. One is this function's body-local
// slot `Data(__local_<fn>__*)`, and the other is a zero-page byte of
// the register allocator's pool that no user zero-page global occupies.
// Both are dead when the function returns. A user static or zero-page
// global stays observable after the return, so the rewrite, which
// leaves the byte holding its old value, can't apply to it.
//
// The walk follows every branch and jump, so a read that only a loop's
// back edge reaches counts. It mirrors `asm_dead_store._is_dead_cfg`.
// That walk treats a `__local_*` slot as live at a return unless it
// gets the slot-symbol map, which this pass isn't given, so the exit
// rule is applied here instead.
bool tmp_dead_after(const Instructions& instrs, const LabelMap& labels, std::size_t idx,
                    const ir::Type_operand& tmp, const std::string& fn_name,
                    const AsmContext& ctx) {
  const std::string prefix = "__local_" + fn_name + "__";
  if (const auto* d = as<ir::Data>(tmp)) {
    if (!starts_with(d->name, prefix)) {
      return false;
    }
  } else if (const auto* zp = as<ir::ZP>(tmp)) {
    const asdl::Int addr = zp->address + zp->offset;
    if (!(0x80 <= addr && addr < 0x100) || ctx.is_user_zp_byte(addr)) {
      return false;
    }
  } else {
    return false;
  }
  const NameSet taken = asm_dead_store::collect_address_taken_names(instrs);
  const bool is_private = asm_dead_store::call_cannot_touch(tmp, prefix, taken);
  std::set<std::size_t> visited;
  std::vector<std::size_t> stack = asm_dead_store::successors(instrs, idx, labels);
  while (!stack.empty()) {
    const std::size_t j = stack.back();
    stack.pop_back();
    if (visited.contains(j)) {
      continue;
    }
    visited.insert(j);
    const auto& nxt = instrs[j];
    auto push_successors = [&] {
      for (const std::size_t s : asm_dead_store::successors(instrs, j, labels)) {
        stack.push_back(s);
      }
    };
    if (const auto* call = as<ir::Call>(nxt);
        call != nullptr && call->name != "icall" && is_private) {
      push_successors();
      continue;
    }
    if (asm_dead_store::is_opaque(nxt)) {
      return false;
    }
    if (const auto* jump = as<ir::Jump>(nxt); jump != nullptr && !labels.contains(jump->target)) {
      if (!asm_dead_store::is_dead_at_tail_call(tmp, &taken)) {
        return false;
      }
      continue;
    }
    if (isa<ir::Ret, ir::Return>(nxt)) {
      continue;
    }
    if (asm_dead_store::reads(nxt, tmp, ctx, nullptr, is_private)) {
      return false;
    }
    if (asm_dead_store::writes_same(nxt, tmp)) {
      continue;
    }
    push_successors();
  }
  return true;
}

std::optional<Match> match_pattern(const Instructions& instrs, const LabelMap& labels,
                                   std::size_t i, const std::string& fn_name,
                                   const AsmContext& ctx) {
  if (i + 4 >= instrs.size()) {
    return std::nullopt;
  }
  // a: Mov(IndexedData(_, index=X), Reg(A)), LDA m,X.
  if (!is_load_indexed_by_x(instrs[i])) {
    return std::nullopt;
  }
  const ir::Type_operand& indexed_src = std::get<ir::Mov>(instrs[i]).src;
  // b: Mov(Reg(A), <Data|ZP>), STA tmp.
  const auto* b = as<ir::Mov>(instrs[i + 1]);
  if (b == nullptr || b->is_volatile) {
    return std::nullopt;
  }
  if (!(is_reg_a(b->src) && isa<ir::Data, ir::ZP>(b->dst))) {
    return std::nullopt;
  }
  const ir::Type_operand& tmp = b->dst;
  // c: Inc(tmp) or Dec(tmp).
  bool is_dec = false;
  if (const auto* inc = as<ir::Inc>(instrs[i + 2])) {
    if (inc->dst != tmp) {
      return std::nullopt;
    }
  } else if (const auto* dec = as<ir::Dec>(instrs[i + 2])) {
    if (dec->dst != tmp) {
      return std::nullopt;
    }
    is_dec = true;
  } else {
    return std::nullopt;
  }
  // d: Mov(tmp, Reg(A)), LDA tmp.
  const auto* d = as<ir::Mov>(instrs[i + 3]);
  if (d == nullptr || d->is_volatile) {
    return std::nullopt;
  }
  if (!(d->src == tmp && is_reg_a(d->dst))) {
    return std::nullopt;
  }
  // e: Compare(Reg(A), <Imm|Data|ZP>), CMP other.
  const auto* e = as<ir::Compare>(instrs[i + 4]);
  if (e == nullptr || !is_reg_a(e->left) || !cpy_addressable(e->right)) {
    return std::nullopt;
  }
  // The soundness gates.
  if (!a_dead_at(instrs, labels, i + 5)) {
    return std::nullopt;
  }
  if (!y_dead_at(instrs, labels, i)) {
    return std::nullopt;
  }
  if (!tmp_dead_after(instrs, labels, i + 4, tmp, fn_name, ctx)) {
    return std::nullopt;
  }
  return Match{.src = indexed_src, .is_dec = is_dec, .other = e->right};
}

// Returns the match of the same step done in A, which `shift_in_a`
// makes of `STA tmp; INC tmp; LDA tmp` when C and V are dead:
//
//     LDA m,X; CLC; ADC #1; CMP other   ->   LDY m,X; INY; CPY other
//
// and the `SEC; SBC #1` form with `DEY`. Y must be dead before and A
// after, as for the five-atom shape. `CMP` and `CPY` set N, Z, and C
// alike, and the `ADC`'s V, which `CPY` doesn't set, must be dead.
// Returns what `match_pattern` does, or nothing.
std::optional<Match> match_in_a(const Instructions& instrs, const LabelMap& labels,
                                std::size_t i) {
  if (i + 3 >= instrs.size()) {
    return std::nullopt;
  }
  if (!is_load_indexed_by_x(instrs[i])) {
    return std::nullopt;
  }
  const auto& b = instrs[i + 1];
  const auto& c = instrs[i + 2];
  const ir::Type_operand* step_src = nullptr;
  const ir::Type_operand* step_dst = nullptr;
  bool is_dec = false;
  if (const auto* add = as<ir::Add>(c); add != nullptr && isa<ir::ClearCarry>(b)) {
    step_src = &add->src;
    step_dst = &add->dst;
  } else if (const auto* sub = as<ir::Sub>(c); sub != nullptr && isa<ir::SetCarry>(b)) {
    step_src = &sub->src;
    step_dst = &sub->dst;
    is_dec = true;
  } else {
    return std::nullopt;
  }
  const auto* imm = as<ir::Imm>(*step_src);
  if (!(imm != nullptr && imm->value == 1 && is_reg_a(*step_dst))) {
    return std::nullopt;
  }
  const auto* e = as<ir::Compare>(instrs[i + 3]);
  if (!(e != nullptr && is_reg_a(e->left) && cpy_addressable(e->right))) {
    return std::nullopt;
  }
  const std::size_t after = i + 4;
  if (!(a_dead_at(instrs, labels, after) && v_dead_at(instrs, labels, after) &&
        y_dead_at(instrs, labels, i))) {
    return std::nullopt;
  }
  return Match{.src = std::get<ir::Mov>(instrs[i]).src, .is_dec = is_dec, .other = e->right};
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  const ir::Type_operand reg_y = ir::Reg{.reg = ir::Y{}};
  Instructions out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::optional<Match> match = match_pattern(instrs, labels, i, fn.name, ctx);
    std::size_t width = 5;
    if (!match) {
      match = match_in_a(instrs, labels, i);
      width = 4;
    }
    if (match) {
      out.push_back(ir::Mov{.src = match->src, .dst = reg_y, .is_volatile = false});
      if (match->is_dec) {
        out.push_back(ir::Dec{.dst = reg_y});
      } else {
        out.push_back(ir::Inc{.dst = reg_y});
      }
      out.push_back(ir::Compare{.left = reg_y, .right = match->other});
      i += width;
      changed = true;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_index_inc_cmp_y(asm_ast::Program prog, const AsmContext& ctx) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx);
    }
  }
  return prog;
}

}  // namespace c6502::passes
