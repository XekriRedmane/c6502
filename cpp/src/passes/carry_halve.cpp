// Halves a 9-bit sum with a `ROR` of its low byte, taking bit 8 from C.
//
// The pattern
//
// `(x + y) / 2` or `(x + y) >> 1` with `x` and `y` bytes (or one of them
// a constant below 256) promotes both to `int`, so the sum has a high
// byte, which is only the carry out of the low bytes' add. When the
// result is truncated to a byte, `tac_to_asm` and the asm optimizer
// leave the high byte's add and its shift:
//
//     ADC   s                 ; the low byte of the sum, C = bit 8
//     STA   t
//     LDA   #$00
//     ADC   #$00              ; the high byte, 0 + 0 + C
//     LSR                     ; C = bit 8 again, A = 0
//     LDA   t
//     ROR                     ; A = bit 8 and bits 7-1 of the sum
//
// `print_centered_plus_col` in `examples/ali_baba/text.c` rounds
// `(space + 1) / 2` that way. The carry out of the `ADC` is already bit
// 8, so the pass drops the four instructions between it and the `ROR`:
//
//     ADC   s
//     ROR
//
// Soundness
//
// After the `ADC s`, A is the low byte `L` of the sum and C its bit 8,
// `B`. In the original, `LDA #$00; ADC #$00` makes A = `B` and clears C
// and V, `LSR` makes C = `B` and A = 0, `LDA t` makes A = `L`, and `ROR`
// makes A = `B << 7 | L >> 1` and C = bit 0 of `L`, with N and Z from
// the new A. The rewrite makes the same A, C, N, and Z from the same `L`
// and `B`. The differences are these.
//
//   * V, which the original leaves clear (from `ADC #$00`) and the
//     rewrite leaves as the `ADC s` set it. V must be dead after the
//     `ROR` (`v_dead_at`).
//   * `t`, which the rewrite doesn't write. `t` must be `Data` or `ZP`
//     and dead after the `LDA t` on every path
//     (`asm_dead_store::mem_dead_after`), and the `LDA t` mustn't be
//     volatile.
//
// The seven instructions are consecutive, so no label or branch is
// between them. The `ADC s` stays as it was, with whatever set C before
// it, so a `SEC` in front of it (`(x + y + 1) / 2` folded into the
// carry) works the same. `s` is read once, as before.
//
// The `LDA #$00` may be a `TXA` or `TYA`, which
// `redundant_load_elimination` makes of it when X or Y holds 0. The
// pass takes one when the last instruction in the block before it that
// can write the register is `LDX #$00` or `LDY #$00`.
//
// Where it runs
//
// In the asm peephole fixed point, which both pipelines run. The high
// byte is often stored and loaded again (`STA u; LDA u`) between the
// `ADC #$00` and the `LSR` in the first sweep, and
// `round_trip_load_drop` removes that pair for a later sweep.
#include "passes/carry_halve.h"

#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kZero = ir::Imm{.value = 0};

// Returns whether `reg` (X or Y) holds 0 at `instrs[idx]`, because the
// last instruction before it in its block that can write `reg` is
// `Mov(Imm(0), reg)`. A label ends the search, since another path may
// reach it.
bool holds_zero(const std::vector<ir::Type_instruction>& instrs, std::size_t idx,
                const ir::Type_operand& reg) {
  const bool is_x = isa<ir::X>(std::get<ir::Reg>(reg).reg);
  for (std::size_t k = idx; k-- > 0;) {
    const ir::Type_instruction& ins = instrs[k];
    if (const auto* m = as<ir::Mov>(ins); m != nullptr && m->dst == reg) {
      return m->src == kZero && !m->is_volatile;
    }
    if (isa<ir::Label, ir::Call, ir::Jump, ir::JumpTable, ir::Phi>(ins)) {
      return false;
    }
    if (const auto* x = as<ir::Inc>(ins); x != nullptr && x->dst == reg) {
      return false;
    }
    if (const auto* x = as<ir::Dec>(ins); x != nullptr && x->dst == reg) {
      return false;
    }
    if (is_x && kills_x(ins)) {
      return false;
    }
    if (!is_x && (kills_y(ins) || sets_up_y(ins))) {
      return false;
    }
  }
  return false;
}

// Returns whether `instrs[idx]` loads A with 0: `LDA #$00`, or `TXA` /
// `TYA` with that register holding 0 (`holds_zero`).
bool is_zero_load(const std::vector<ir::Type_instruction>& instrs, std::size_t idx) {
  const auto* m = as<ir::Mov>(instrs[idx]);
  if (m == nullptr || m->dst != kA) {
    return false;
  }
  if (m->src == kZero) {
    return true;
  }
  const auto* r = as<ir::Reg>(m->src);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg) && holds_zero(instrs, idx, m->src);
}

// Returns whether `instrs[i]` to `instrs[i + 6]` are `ADC s; STA t;
// LDA #0; ADC #0; LSR A; LDA t; ROR A`, with `t` a `Data` or `ZP` byte,
// and the `LDA #0` possibly a `TXA` or `TYA` of a register holding 0.
bool matches(const std::vector<ir::Type_instruction>& instrs, std::size_t i) {
  const auto* add = as<ir::Add>(instrs[i]);
  const auto* store = as<ir::Mov>(instrs[i + 1]);
  const auto* carry = as<ir::Add>(instrs[i + 3]);
  const auto* lsr = as<ir::LogicalShiftRight>(instrs[i + 4]);
  const auto* load = as<ir::Mov>(instrs[i + 5]);
  const auto* ror = as<ir::RotateRight>(instrs[i + 6]);
  return add != nullptr && add->dst == kA && store != nullptr && store->src == kA &&
         !store->is_volatile && isa<ir::Data, ir::ZP>(store->dst) &&
         is_zero_load(instrs, i + 2) && carry != nullptr && carry->src == kZero &&
         carry->dst == kA && lsr != nullptr && lsr->dst == kA && load != nullptr &&
         load->src == store->dst && load->dst == kA && !load->is_volatile &&
         ror != nullptr && ror->dst == kA;
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx,
                      const SymbolAddrs* zp_slot_symbols) {
  auto& instrs = fn.instructions;
  std::size_t i = 0;
  while (i + 7 <= instrs.size()) {
    if (!(matches(instrs, i) && v_dead_at(instrs, i + 7) &&
          asm_dead_store::mem_dead_after(instrs, i + 5,
                                         std::get<ir::Mov>(instrs[i + 1]).dst, ctx,
                                         fn.name, zp_slot_symbols))) {
      ++i;
      continue;
    }
    // Keeps the `ADC s` and the `ROR` and drops the five between them.
    const auto first = instrs.begin() + static_cast<std::ptrdiff_t>(i) + 1;
    instrs.erase(first, first + 5);
    i += 2;
  }
}

}  // namespace

asm_ast::Program apply_carry_halve(asm_ast::Program prog, const AsmContext& ctx,
                                   const SymbolAddrs* zp_slot_symbols) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx, zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
