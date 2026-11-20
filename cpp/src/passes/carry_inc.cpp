// The carry-INC peephole, which adds a carry into a byte, or takes a
// borrow out of one, in place with BCC / INC or BCS / DEC.
//
// The high byte of an add whose other operand's high byte is zero, such
// as `t + (uint16_t)c` or `p + 53`, is lowered by `tac_to_asm` as
//
//     Mov(M, A); Add(Imm(0), A); Mov(A, M)      LDA M; ADC #0; STA M
//
// which adds the carry out of the low byte into M. When M is written in
// place, hand-written 6502 code does the same thing with
//
//     Branch(CC, skip); Inc(M); Label(skip)     BCC skip; INC M; skip:
//
// A subtract's high byte, `LDA M; SBC #0; STA M`, takes 1 - C out of M,
// and becomes `BCS skip; DEC M; skip:` the same way.
//
// A wider add has one carry byte for each byte above the lowest, each
// reading the carry out of the one before. When all of them are in
// place, the run becomes the carry chain that `inc_peephole` uses for
// `x += 1`:
//
//     BCC done; INC M1; BNE done; INC M2; BNE done; INC M3; done:
//
// since a byte that INC wraps to zero is the one case that carries into
// the next. Only the top byte of a subtract's run is rewritten, because
// a byte that DEC wraps to $FF can't be detected with one branch.
//
// That's 2 instructions instead of 3, and 4 bytes instead of 6 in zero
// page (5 instead of 8 at an absolute address). It takes 3 cycles when
// the branch is taken and 7 when it isn't (8 for an absolute INC or
// DEC), against 8 (10 absolute) for the ADC form. The rewrite also
// applies to `Reg(X)` and `Reg(Y)`, where `TXA; ADC #0; TAX` becomes
// `BCC skip; INX`.
//
// Move coalescing gives M the same byte on both sides where it can (see
// "Carry bytes" in `passes/optimization_asm/coalescing.py`), and an add
// to a global in place, `g += c`, has this shape already.
//
// The two forms leave different values behind, so the rewrite needs all
// of these to be dead after the store:
//
//   * A. The ADC form leaves M's new value in A, and the INC form
//     leaves A as it was before the LDA.
//   * C. The ADC form sets C from the carry out of M, and the INC form
//     leaves C as it came in. A following ADC that continues the chain
//     into another byte reads C, so a wider add only has its top byte
//     rewritten.
//   * N, Z, and V. They come from the ADC in one form, and from the INC
//     or the low byte's ADC in the other.
//
// A carry byte in X or Y (`hwreg_eligibility`'s carry hint) may also
// have C read after it, as `while (s >= k) s -= k;` reads the borrow
// out of its 16-bit `s` with `BCC`. The rewrite then keeps C as the
// ADC / SBC form leaves it:
//
//     TXA; SBC #0; TAX    ->   BCS skip; CPX #1; DEX; skip:
//     TXA; ADC #0; TAX    ->   BCC skip; INX; BEQ skip; CLC; skip:
//
// With C set coming in, nothing is borrowed (or carried) and the branch
// skips with C as it was. Otherwise `CPX #1` sets C exactly when X
// isn't zero, which is when the decrement doesn't borrow, and `DEX`
// leaves C alone. An `INX` that wraps X to zero carries out, and any
// other leaves C clear. That's Zork's `BCS .inc_track; DEX` in
// `do_rwts_on_sector`.
//
// `asm_liveness.a_dead_at` and `flags_dead_at` (N and Z) check those
// conditions along every path from the store, and `c_dead_at` and
// `v_dead_at` check C and V the same way, each on its own. A `BEQ`
// after the `LDA` / `ORA` that tests a 16-bit counter for zero reads
// N and Z, which the LDA set, and doesn't make C or V live. A volatile M
// is left alone, as in `inc_peephole`, since INC is one read-modify-
// write access where the ADC form reads and writes separately.
//
// The pass runs in the asm peephole fixed point, after
// `replace_pseudoregisters`, when the operands are concrete and it's
// known whether M is a zero-page or absolute byte that INC can address.
#include "passes/carry_inc.h"

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/inc_peephole.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using inc_peephole::is_inc_eligible_operand;
using inc_peephole::operands_equal;

constexpr std::string_view kLabelPrefix = ".carry_inc@";

bool is_reg_a_op(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_imm_zero(const ir::Type_operand& op) {
  const auto* imm = as<ir::Imm>(op);
  return imm != nullptr && imm->value == 0;
}

// Returns the number of a label named `.carry_inc@<digits>`, or
// nothing for any other name (Python's `_LABEL_RE`).
std::optional<long long> label_number(std::string_view name) {
  if (!name.starts_with(kLabelPrefix)) {
    return std::nullopt;
  }
  const std::string_view digits = name.substr(kLabelPrefix.size());
  if (digits.empty()) {
    return std::nullopt;
  }
  long long n = 0;
  for (char c : digits) {
    if (c < '0' || c > '9') {
      return std::nullopt;
    }
    n = n * 10 + (c - '0');
  }
  return n;
}

// One carry byte: its operand, and whether it is an add.
struct CarryByte {
  const ir::Type_operand* mem = nullptr;
  bool is_add = false;
};

// Returns the carry byte at `instrs[i..i+3)` when it is
// `Mov(M, A); Add(Imm(0), A); Mov(A, M)` (an add) or the same with
// `Sub`, with M an operand INC takes and neither move volatile.
std::optional<CarryByte> carry_byte(Instrs instrs, std::size_t i) {
  if (i + 3 > instrs.size()) {
    return std::nullopt;
  }
  const auto* load = as<ir::Mov>(instrs[i]);
  if (load == nullptr || load->is_volatile || !is_reg_a_op(load->dst) ||
      !is_inc_eligible_operand(load->src)) {
    return std::nullopt;
  }
  bool is_add = false;
  if (const auto* add = as<ir::Add>(instrs[i + 1])) {
    if (!is_imm_zero(add->src) || !is_reg_a_op(add->dst)) {
      return std::nullopt;
    }
    is_add = true;
  } else if (const auto* sub = as<ir::Sub>(instrs[i + 1])) {
    if (!is_imm_zero(sub->src) || !is_reg_a_op(sub->dst)) {
      return std::nullopt;
    }
  } else {
    return std::nullopt;
  }
  const auto* store = as<ir::Mov>(instrs[i + 2]);
  if (store == nullptr || store->is_volatile || !is_reg_a_op(store->src) ||
      !operands_equal(store->dst, load->src)) {
    return std::nullopt;
  }
  return CarryByte{&load->src, is_add};
}

struct Match {
  std::vector<ir::Type_operand> mems;
  bool is_add = false;
  bool keep_c = false;
};

// Returns the run of carry bytes in place that starts at `instrs[i]`,
// with A and every flag dead after the run, or every flag but C when
// the run is one byte in X or Y (`keep_c`). An add's run takes every
// carry byte of an add in a row, and a subtract's run is one byte.
std::optional<Match> match_run(Instrs instrs, const LabelMap& labels, std::size_t i) {
  const std::optional<CarryByte> first = carry_byte(instrs, i);
  if (!first) {
    return std::nullopt;
  }
  Match m;
  m.mems.push_back(*first->mem);
  m.is_add = first->is_add;
  if (m.is_add) {
    while (true) {
      const std::optional<CarryByte> next = carry_byte(instrs, i + 3 * m.mems.size());
      if (!next || !next->is_add) {
        break;
      }
      m.mems.push_back(*next->mem);
    }
  }
  const std::size_t end = i + 3 * m.mems.size();
  if (!a_dead_at(instrs, labels, end) || !flags_dead_at(instrs, labels, end) ||
      !v_dead_at(instrs, labels, end)) {
    return std::nullopt;
  }
  if (c_dead_at(instrs, labels, end)) {
    m.keep_c = false;
    return m;
  }
  if (m.mems.size() == 1 && isa<ir::Reg>(m.mems[0])) {
    m.keep_c = true;
    return m;
  }
  return std::nullopt;
}

// Rewrites the runs in one function, numbering the labels from
// `counter`, which it leaves at the next free number.
void rewrite_function(ir::Function& fn, long long& counter) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::optional<Match> match = match_run(instrs, labels, i);
    if (!match) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    const std::string skip = std::string(kLabelPrefix) + std::to_string(counter);
    ++counter;
    const ir::Type_operand& m0 = match->mems[0];
    if (match->keep_c && match->is_add) {
      out.push_back(ir::Branch{.cond = ir::CC{}, .target = skip});
      out.push_back(ir::Inc{.dst = m0});
      out.push_back(ir::Branch{.cond = ir::EQ{}, .target = skip});
      out.push_back(ir::ClearCarry{});
    } else if (match->keep_c) {
      out.push_back(ir::Branch{.cond = ir::CS{}, .target = skip});
      out.push_back(ir::Compare{.left = m0, .right = ir::Imm{.value = 1}});
      out.push_back(ir::Dec{.dst = m0});
    } else if (match->is_add) {
      out.push_back(ir::Branch{.cond = ir::CC{}, .target = skip});
      for (std::size_t k = 0; k < match->mems.size(); ++k) {
        if (k != 0) {
          out.push_back(ir::Branch{.cond = ir::NE{}, .target = skip});
        }
        out.push_back(ir::Inc{.dst = match->mems[k]});
      }
    } else {
      out.push_back(ir::Branch{.cond = ir::CS{}, .target = skip});
      out.push_back(ir::Dec{.dst = m0});
    }
    out.push_back(ir::Label{.name = skip});
    changed = true;
    i += 3 * match->mems.size();
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_carry_inc_peephole(asm_ast::Program prog) {
  // The labels are numbered across the whole program, past any from an
  // earlier run of the fixed point. dasm scopes a `.` label to its
  // SUBROUTINE, but the simulator's assembler (`sim/assembler.py`)
  // rejects the same label in two functions.
  long long counter = 0;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      for (const auto& ins : fn->instructions) {
        if (const auto* label = as<ir::Label>(ins)) {
          if (auto n = label_number(label->name)) {
            counter = std::max(counter, *n + 1);
          }
        }
      }
    }
  }
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, counter);
    }
  }
  return prog;
}

}  // namespace c6502::passes
