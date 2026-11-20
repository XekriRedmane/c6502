// Folds `LDA src; STA tmp; LDA other; CMP tmp` into `LDA other; CMP
// src`, and the same with `ADC`, `SBC`, `AND`, `ORA`, or `EOR` for
// `CMP`.
//
// The pattern
//
// Four consecutive instructions:
//
//     Mov(<src>,         Reg(A))   // LDA src
//     Mov(Reg(A),        <tmp>)    // STA tmp
//     Mov(<other>,       Reg(A))   // LDA other
//     Compare(Reg(A),    <tmp>)    // CMP tmp
//
// The last may instead be `Add` / `Sub` / `And` / `Or` of `tmp` into A,
// or `Xor` of A and `tmp`, with an optional `SetCarry` / `ClearCarry`
// in front of it (five instructions), which becomes `LDA other; [SEC;]
// SBC src`. `do_rwts_on_sector` in `examples/zork/disk.c` subtracts a
// zero-extended global that way:
//
//     LDA   SECTORS_PER_TRACK
//     STA   __local_do_rwts_on_sector__command
//     LDA   __local_do_rwts_on_sector__sector_0
//     SEC
//     SBC   __local_do_rwts_on_sector__command
//
// becomes `LDA __local_do_rwts_on_sector__sector_0; SEC; SBC
// SECTORS_PER_TRACK`. For these the source must be one the instruction
// can address: an immediate, `Data` / `ZP`, `abs,X` / `abs,Y`
// (`asm_emit._emit_acc_arith_src`), or `(zp),Y` with Y already loaded
// (`IndirectZpY`). The last takes the same Y-safe `other` as a compare
// of one (see Eligibility), so Y still holds the offset at the `SBC`,
// and the `SEC` or `CLC` between doesn't change Y.
// `print_centered_plus_col` in `examples/ali_baba/text.c` computes
// `40 - s[0]` as `LDY #$00; LDA #$28; SEC; SBC (s),Y`. An `IndirectZp`
// source, which loads its own Y, is left alone.
//
// The middle pair stages `src`'s value through a temporary in memory so
// that the final CMP can compare A (which holds other) with the staged
// copy. CMP takes every addressing mode LDA takes (and `abs,X` /
// `abs,Y`, which `asm_emit._emit_compare` writes for IndexedData), so
// the temporary can go:
//
//     Mov(<other>,       Reg(A))   // LDA other
//     Compare(Reg(A),    <src>)    // CMP src
//
// Two instructions take the place of four. The CMP reads `src`
// directly, and sets Z, N, C, and V as `CMP tmp` did, since tmp held
// src's value.
//
// The case that called for it
//
// `beam_target_tick.c`, `beam_y == floor_y_table[floor_idx]`:
//
//     AND   #$0F
//     TAX
//     LDA   floor_y_table,X
//     STA   __local_beam_target_tick__0
//     LDA   beam_y
//     CMP   __local_beam_target_tick__0
//     BNE   .if_end@2
//
// The equivalent written by hand, which this pass produces, is
//
//     AND   #$0F
//     TAX
//     LDA   beam_y
//     CMP   floor_y_table,X
//     BNE   .if_end@2
//
// That drops 5 bytes and 8 cycles each time (the LDA src and STA tmp
// pair). The slot is freed too, since `prune_unused_slots` collects
// the `__local_*` symbol once nothing names it.
//
// Eligibility
//
//   * All four instructions are present and not volatile.
//   * `tmp` is `Data` or `ZP`, memory that can be compared by its
//     fields.
//   * `src` is an addressing mode CMP takes that doesn't depend on a
//     register the LDA between might write:
//       - `Imm` / `ImmLabelLow` / `ImmLabelHigh`, which read no
//         register.
//       - `Data` / `ZP`, which read no register.
//       - `IndexedData(_, _, X)`, since no LDA mode writes X.
//       - `IndexedData(_, _, Y)`, whose Y `LDA other` can write when
//         other is `Frame` / `Stack` / `Indirect` / `IndirectZp` (those
//         are written with an `LDY #off` setup). It is allowed only
//         when other is none of those.
//       - `IndirectY` / `IndirectZpY` (`(DPTR),Y` / `(zp),Y` with Y
//         already loaded) and `IndirectZp` (which loads its own Y).
//         These need the same Y-safe `other` as `IndexedData(_, _,
//         Y)`, and `other` is a plain read that can't move the pointer.
//         This is the `LDA key; CMP (p),Y` shape of comparing a byte
//         with one read through a pointer.
//   * `other` doesn't alias `tmp`. To be safe, `other` must be `Imm` /
//     `ImmLabelLow/High`, or a `Data` / `ZP` that differs from `tmp`
//     in its fields. Indirect / IndexedData / Frame / Stack /
//     IndirectY / IndirectZpY / IndirectZp are rejected, because the
//     byte they read depends on registers and pointers at run time.
//   * `tmp` and `src` may be the same. A self-store `LDA M; STA M; LDA
//     other; CMP M` folds soundly too.
//   * `tmp` is dead after the CMP on every path forward
//     (`asm_dead_store::mem_dead_after`), so dropping its store loses
//     nothing. Register allocation reuses a `__local_*` slot for
//     unrelated values, so this is checked for each candidate. When it
//     isn't dead, the group rule below can still apply.
//   * Otherwise, `tmp` must be one of the function's own `__local_*`
//     slots whose address it never takes, since the other readers of a
//     global are in other functions, and every read of `tmp` in the
//     function must be "owned" by a matching candidate window, which
//     means it sits at one of the candidates' CMP positions. Then
//     folding every candidate of the group together leaves no reader
//     orphaned. Writes elsewhere are fine, since asm_dead_store cleans
//     up the orphaned stores. These are the cases the rule guards
//     against:
//       - The back-to-back column-band test in `beam_target_draw`
//         (`screen_col > $25` then `screen_col <= $23`). One STA tmp
//         feeds two `CMP tmp` reads, and only one is matched, so the
//         second has no owner and the whole group stays.
//       - Three independent `col == floor_*[idx]` comparisons in
//         `do_ascend` with the same staged slot. Each comparison has
//         its own STA tmp and CMP tmp, so every read has an owner, and
//         the group folds.
//
// The rewrite drops both the LDA src and the STA tmp. After it, A
// holds other as before, the flags after the CMP are the ones the
// original CMP set, and no write to memory that later code reads is
// lost, since the check above shows `tmp` is read nowhere else.
//
// Why other passes don't cover it
//
//   * `asm_dead_store` would drop the STA only if the LDA src weren't
//     needed for A's value, and the CMP reads A. That pass can't see
//     that the value reaches A only to be replaced by the LDA other.
//   * `redundant_load_elimination` tracks what A holds, but the STA
//     sequence ends the tracking before the CMP runs.
//   * `mem_const_prop` substitutes only immediates.
//
// Where it runs
//
// Inside the peephole fixed point. The shorter sequence gives later
// passes more to find (`dead_a_arith` may drop further LDAs after it).
#include "passes/cmp_through_tmp_fold.h"

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_stable_mem(const ir::Type_operand& op) { return isa<ir::Data, ir::ZP>(op); }

bool operands_equal(const ir::Type_operand& a, const ir::Type_operand& b) {
  if (const auto* da = as<ir::Data>(a)) {
    const auto* db = as<ir::Data>(b);
    return db != nullptr && da->name == db->name && da->offset == db->offset;
  }
  if (const auto* za = as<ir::ZP>(a)) {
    const auto* zb = as<ir::ZP>(b);
    return zb != nullptr && za->address == zb->address && za->offset == zb->offset;
  }
  return false;
}

// Returns whether CMP can address `src` without depending on a register
// that the LDA between might write. For `IndexedData(_, _, Y)` the
// caller also checks `other`.
bool src_eligible(const ir::Type_operand& src) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP, ir::IndexedData,
             ir::IndirectY, ir::IndirectZpY, ir::IndirectZp>(src);
}

// Returns whether `Mov(other, Reg(A))` is written without an LDY setup,
// so it doesn't write Y. Checked when src is indexed by Y.
bool y_safe_other(const ir::Type_operand& other) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP, ir::IndexedData,
             ir::IndirectY, ir::IndirectZpY>(other);
}

// Returns whether `other` can be shown apart from `tmp` by comparing
// their fields. An indirect, indexed, or frame-relative `other` is
// rejected, because the byte it reads depends on registers or pointers
// at run time.
bool other_alias_safe(const ir::Type_operand& other, const ir::Type_operand& tmp) {
  if (isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(other)) {
    return true;
  }
  if (isa<ir::Data, ir::ZP>(other)) {
    return !operands_equal(other, tmp);
  }
  return false;
}

// Returns the operands of `instr` that read memory. Destinations that
// are only written don't count, since overwriting tmp elsewhere is
// harmless once the matched CMP is gone (asm_dead_store cleans up the
// orphaned store). The read-modify-write positions (Inc, Dec, the
// shifts, and the dst of Add / Sub / And / Or / Xor, which is normally
// Reg(A) but counts as a read here to be safe) do count.
std::vector<ir::Type_operand> read_operands(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      return decimal_arith::reads(std::get<ir::DecimalArith>(instr));
    case ir::Kind_instruction::Mov:
      return {std::get<ir::Mov>(instr).src};
    case ir::Kind_instruction::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      return {c.left, c.right};
    }
    case ir::Kind_instruction::Add: {
      const auto& a = std::get<ir::Add>(instr);
      return {a.src, a.dst};
    }
    case ir::Kind_instruction::Sub: {
      const auto& a = std::get<ir::Sub>(instr);
      return {a.src, a.dst};
    }
    case ir::Kind_instruction::And: {
      const auto& a = std::get<ir::And>(instr);
      return {a.src, a.dst};
    }
    case ir::Kind_instruction::Or: {
      const auto& a = std::get<ir::Or>(instr);
      return {a.src, a.dst};
    }
    case ir::Kind_instruction::Xor: {
      // Xor is the one ALU atom with two named sources.
      const auto& x = std::get<ir::Xor>(instr);
      return {x.src1, x.src2, x.dst};
    }
    case ir::Kind_instruction::Push:
      return {std::get<ir::Push>(instr).src};
    case ir::Kind_instruction::Inc:
      return {std::get<ir::Inc>(instr).dst};
    case ir::Kind_instruction::Dec:
      return {std::get<ir::Dec>(instr).dst};
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return {std::get<ir::ArithmeticShiftLeft>(instr).dst};
    case ir::Kind_instruction::LogicalShiftRight:
      return {std::get<ir::LogicalShiftRight>(instr).dst};
    case ir::Kind_instruction::RotateLeft:
      return {std::get<ir::RotateLeft>(instr).dst};
    case ir::Kind_instruction::RotateRight:
      return {std::get<ir::RotateRight>(instr).dst};
    case ir::Kind_instruction::BitTest:
      return {std::get<ir::BitTest>(instr).src};
    default:
      return {};
  }
}

bool reads_tmp(const ir::Type_instruction& instr, const ir::Type_operand& tmp) {
  for (const auto& op : read_operands(instr)) {
    if (operands_equal(op, tmp)) {
      return true;
    }
  }
  return false;
}

// Returns the operand that the last instruction of the pattern reads
// into A or compares A with, or null.
const ir::Type_operand* tmp_read(const ir::Type_instruction& d) {
  if (const auto* c = as<ir::Compare>(d)) {
    return is_reg_a(c->left) ? &c->right : nullptr;
  }
  if (const auto* x = as<ir::Xor>(d)) {
    if (!is_reg_a(x->dst)) {
      return nullptr;
    }
    if (is_reg_a(x->src1)) {
      return &x->src2;
    }
    if (is_reg_a(x->src2)) {
      return &x->src1;
    }
    return nullptr;
  }
  return std::visit(
      [](const auto& i) -> const ir::Type_operand* {
        using T = std::decay_t<decltype(i)>;
        if constexpr (std::is_same_v<T, ir::Add> || std::is_same_v<T, ir::Sub> ||
                      std::is_same_v<T, ir::And> || std::is_same_v<T, ir::Or>) {
          return is_reg_a(i.dst) ? &i.src : nullptr;
        } else {
          return nullptr;
        }
      },
      d);
}

// Returns `d` reading `src` where it read the temporary.
ir::Type_instruction with_src(const ir::Type_instruction& d, const ir::Type_operand& src) {
  if (const auto* c = as<ir::Compare>(d)) {
    return ir::Compare{.left = c->left, .right = src};
  }
  if (const auto* x = as<ir::Xor>(d)) {
    if (is_reg_a(x->src1)) {
      return ir::Xor{.src1 = x->src1, .src2 = src, .dst = x->dst};
    }
    return ir::Xor{.src1 = src, .src2 = x->src2, .dst = x->dst};
  }
  ir::Type_instruction out = d;
  std::visit(
      [&](auto& i) {
        if constexpr (requires { i.src; i.dst; }) {
          i.src = src;
        }
      },
      out);
  return out;
}

// Returns whether `ADC` / `SBC` / `AND` / `ORA` / `EOR` can address
// `src` (`asm_emit._emit_acc_arith_src`, less the soft-stack modes and
// the `(zp),Y` that loads its own Y).
bool alu_src_eligible(const ir::Type_operand& src) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP, ir::IndexedData,
             ir::IndirectZpY>(src);
}

// Returns whether (a, b, c, d) is `LDA src; STA tmp; LDA other; <op>
// tmp` that can be folded.
bool matches_pattern(const ir::Type_instruction& a, const ir::Type_instruction& b,
                     const ir::Type_instruction& c, const ir::Type_instruction& d) {
  const ir::Type_operand* tmp_cmp = tmp_read(d);
  const auto* lda = as<ir::Mov>(a);
  const auto* sta = as<ir::Mov>(b);
  const auto* ldo = as<ir::Mov>(c);
  // A volatile `src` is still read once, by the CMP (or ALU op). It
  // moves past the load of `other`, so that load mustn't be volatile
  // too. `dump_buffer_with_more`'s `++CURR_LINE >= WNDBTM`, with the
  // Monitor's volatile WNDBTM, becomes `CMP WNDBTM`.
  if (!(lda != nullptr && (!lda->is_volatile || (ldo != nullptr && !ldo->is_volatile)) &&
        is_reg_a(lda->dst) && sta != nullptr && !sta->is_volatile && is_reg_a(sta->src) &&
        is_stable_mem(sta->dst) && ldo != nullptr && !ldo->is_volatile && is_reg_a(ldo->dst) &&
        tmp_cmp != nullptr && is_stable_mem(*tmp_cmp))) {
    return false;
  }
  const ir::Type_operand& src = lda->src;
  const ir::Type_operand& other = ldo->src;
  if (!operands_equal(sta->dst, *tmp_cmp) || !src_eligible(src)) {
    return false;
  }
  if (!isa<ir::Compare>(d) && !alu_src_eligible(src)) {
    return false;
  }
  // A src that reads Y rules out the `other` shapes written with an
  // LDY.
  const auto* indexed = as<ir::IndexedData>(src);
  const bool reads_y = (indexed != nullptr && isa<ir::Y>(indexed->index)) ||
                       isa<ir::IndirectY, ir::IndirectZpY, ir::IndirectZp>(src);
  if (reads_y && !y_safe_other(other)) {
    return false;
  }
  return other_alias_safe(other, sta->dst);
}

// Returns the names of `fn`'s own `__local_<fn>__*` slots whose address
// `fn` never takes (no `ImmLabelLow` / `ImmLabelHigh` of them), which no
// other function reads.
std::set<std::string> own_private_slots(const ir::Function& fn) {
  const std::string prefix = "__local_" + fn.name + "__";
  std::set<std::string> names;
  std::set<std::string> taken;
  for (const auto& ins : fn.instructions) {
    for_each_operand<false>(ins, [&](const ir::Type_operand& op) {
      if (const auto* d = as<ir::Data>(op)) {
        if (d->name.starts_with(prefix)) {
          names.insert(d->name);
        }
      } else if (const auto* lo = as<ir::ImmLabelLow>(op)) {
        taken.insert(lo->name);
      } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
        taken.insert(hi->name);
      }
    });
  }
  for (const auto& t : taken) {
    names.erase(t);
  }
  return names;
}

// A window that matched: the LDA src at `start`, and the CMP (or ALU
// op) that reads tmp at `op`, which is `start + 3`, or `start + 4`
// after a SEC / CLC.
struct Candidate {
  std::size_t start;
  std::size_t op;
};

void rewrite_function(ir::Function& fn, const AsmContext& ctx,
                      const SymbolAddrs* zp_slot_symbols) {
  const Instrs instrs = fn.instructions;
  const std::size_t n = instrs.size();
  auto src_of = [&](const Candidate& c) -> const ir::Type_operand& {
    return std::get<ir::Mov>(instrs[c.start]).src;
  };
  auto tmp_of = [&](const Candidate& c) -> const ir::Type_operand& {
    return std::get<ir::Mov>(instrs[c.start + 1]).dst;
  };

  // The first pass lists every window that matches.
  std::vector<Candidate> candidates;
  for (std::size_t i = 0; i + 3 < n; ++i) {
    std::size_t j = i + 3;
    if (isa<ir::SetCarry, ir::ClearCarry>(instrs[j]) && j + 1 < n &&
        !isa<ir::Compare>(instrs[j + 1])) {
      ++j;
    }
    if (matches_pattern(instrs[i], instrs[i + 1], instrs[i + 2], instrs[j])) {
      candidates.push_back({i, j});
    }
  }
  if (candidates.empty()) {
    return;
  }

  // The second pass groups the candidates by tmp. In a group, every
  // read of tmp in the function must be "owned" by a candidate of the
  // group, which means it sits at one of the candidates' CMP positions.
  // Then folding every candidate of the group leaves no reader orphaned.
  // If even one read of tmp is outside the owned set, no candidate of
  // the group can fold by this rule.
  std::vector<std::pair<ir::Type_operand, std::set<std::size_t>>> groups;
  for (const auto& c : candidates) {
    const ir::Type_operand& tmp = tmp_of(c);
    auto it = groups.begin();
    while (it != groups.end() && it->first != tmp) {
      ++it;
    }
    if (it == groups.end()) {
      groups.emplace_back(tmp, std::set<std::size_t>{});
      it = groups.end() - 1;
    }
    it->second.insert(c.op);
  }
  const std::set<std::string> own = own_private_slots(fn);
  std::vector<ir::Type_operand> foldable;
  for (const auto& [tmp, owned] : groups) {
    // Only the function's own slots, since a global's other readers are
    // in other functions. `g3 = p18; mix(w24 + g3);` in a loop had its
    // store to the zero-page global `g3` dropped, as the add's
    // temporary, since the add was the only read of `g3` in the
    // function (fuzz.stmts, seed 1809).
    const auto* d = as<ir::Data>(tmp);
    if (d == nullptr || own.count(d->name) == 0) {
      continue;
    }
    bool all_owned = true;
    for (std::size_t k = 0; k < n && all_owned; ++k) {
      all_owned = owned.count(k) > 0 || !reads_tmp(instrs[k], tmp);
    }
    if (all_owned) {
      foldable.push_back(tmp);
    }
  }
  auto is_foldable = [&](const ir::Type_operand& tmp) {
    for (const auto& f : foldable) {
      if (f == tmp) {
        return true;
      }
    }
    return false;
  };
  const LabelMap labels(instrs);
  std::vector<const Candidate*> by_start(n, nullptr);
  bool any = false;
  for (const auto& c : candidates) {
    if (is_foldable(tmp_of(c)) ||
        asm_dead_store::mem_dead_after(instrs, labels, c.op, tmp_of(c), ctx, fn.name,
                                       zp_slot_symbols)) {
      by_start[c.start] = &c;
      any = true;
    }
  }
  if (!any) {
    return;
  }

  // The third pass writes the instructions, folding every candidate
  // that passed.
  std::vector<ir::Type_instruction> out;
  out.reserve(n);
  std::size_t i = 0;
  while (i < n) {
    if (const Candidate* c = by_start[i]) {
      out.push_back(ir::Mov{.src = std::get<ir::Mov>(instrs[i + 2]).src,
                            .dst = ir::Reg{.reg = ir::A{}},
                            .is_volatile = false});
      for (std::size_t k = i + 3; k < c->op; ++k) {
        out.push_back(instrs[k]);  // SEC / CLC
      }
      out.push_back(with_src(instrs[c->op], src_of(*c)));
      i = c->op + 1;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_cmp_through_tmp_fold(asm_ast::Program prog, const AsmContext& ctx,
                                            const SymbolAddrs* zp_slot_symbols) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx, zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
