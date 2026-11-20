// Drops dead `PHA` / `PLA` pairs.
//
// `tac_to_asm._translate_indirect_indexed_store` writes the cautious
// shape
//
//     Mov(src, A)         ; LDA src
//     Push(Reg(A))        ; PHA          (saves src across the index load)
//     Mov(idx, A)         ; LDA idx
//     Mov(A, Reg(Y))      ; TAY
//     Pop(Reg(A))         ; PLA          (restores src)
//     Mov(A, IndirectY)   ; STA (DPTR),Y
//
// The PHA/PLA is needed when `idx` lives in `Frame`, where the `LDA idx`
// changes A, which is why the lowering is cautious. But once
// `direct_index_load` fuses `LDA idx; TAY` into `LDY idx` (when `idx`
// is a `Data`, `ZP`, or `Imm` operand), the body between the PHA and the
// PLA no longer touches A, and the pair saves and restores for nothing.
//
// This peephole finds the shape left by the fusion,
//
//     Push(Reg(A))                ; PHA
//     <body that leaves A alone>  ; e.g. LDY Data
//     Pop(Reg(A))                 ; PLA
//
// and drops the PHA/PLA pair. The body's effect on Y, X, memory, and
// the flags is unchanged, and A keeps the value it had before the PHA
// past the deleted PLA, which is what the sequence that saved and
// restored it gave.
//
// Eligibility
//
// The pass matches `Push(Reg(A))` at `i` and scans forward to
// `Pop(Reg(A))` at `j`. The body `instrs[i+1 .. j-1]` must
//
// * not read A. A read in the body would see the same value with or
//   without the pair, but the stricter rule that the body neither reads
//   nor writes A keeps the scan small and avoids a subtle case.
// * not write A. The PLA would restore the old A and lose the body's
//   write, so dropping the pair would leak the write past `j`. This is
//   the one hard constraint, and the others are there for simplicity.
// * not hold another `Push` or `Pop`. A nested pair complicates the
//   match, since the inner one could pair with the outer Pop. The fixed
//   point runs the pass again on the pairs left once an inner pair
//   collapses, so the restriction loses no fold.
// * not hold a `Call` (which changes A and the stack), a `Ret` or
//   `Return` (a transfer of control in the middle), or a `Label`,
//   `Jump`, or `Branch`, since another predecessor could enter the body
//   with A holding a different value.
//
// The N and Z flags the `PLA` set from the saved A must also be dead
// after it, with no `Branch` reading N or Z between `j` and the next
// instruction that sets them, which `flags_dead_at(instrs, j + 1)` walks
// for. PHA and PLA don't change C or V.
//
// Soundness
//
// Before, PHA / body / PLA leaves A at its original value, Y, X, and
// memory as the body left them, and N/Z from the original A (the PLA).
// After, the body alone leaves A at its original value (the body didn't
// change it), Y, X, and memory as before, and N/Z from the body's last
// flag-setter, or unchanged when it has none. Only N and Z can differ,
// and the test that they are dead means nothing reads the difference.
//
// Reloading in place of the pair
//
// When the `PHA` saves a value just loaded from a byte the body doesn't
// change, and the body only moves values into registers,
// `LDA M; PHA; TXA; TAY; PLA` becomes `TXA; TAY; LDA M`
// (`find_reload_pop`), 2 bytes and 5 cycles less. That's a `(zp),Y`
// store of `M` at an index that's in X, as in Zork's `read_line`.
//
// Where it runs
//
// In `_peephole_fixedpoint`, after `direct_index_load`, which turns the
// body into an `LDY Data` that leaves A alone in place of the
// `LDA Data; TAY` that changes it. The peephole runs in both the
// optimized and the unoptimized pipelines, since the save and restore
// are wasted in both and the test is local.
#include "passes/dead_pha_pla.h"

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

// Returns the index of the `Pop(Reg(A))` at or after `start` when the
// body before it leaves A alone and holds nothing that ends the match,
// as the comment at the top lists. Returns nothing otherwise.
std::optional<std::size_t> find_matching_pop(Instrs instrs, std::size_t start) {
  for (std::size_t j = start; j < instrs.size(); ++j) {
    const ir::Type_instruction& instr = instrs[j];
    if (const auto* p = as<ir::Pop>(instr)) {
      if (is_reg_a(p->dst)) {
        return j;
      }
      // A pop into another register doesn't match the stack.
      return std::nullopt;
    }
    if (isa<ir::Push>(instr)) {
      // A nested push. The fixed point tries again after the inner pair
      // collapses.
      return std::nullopt;
    }
    if (isa<ir::Call, ir::Ret, ir::Return, ir::Label, ir::Jump, ir::Branch>(instr)) {
      return std::nullopt;
    }
    if (reads_a(instr) || kills_a(instr)) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

// Returns the index of the `Pop(Reg(A))` that restores the value the
// `Push(Reg(A))` at `i` saves, when that value was just loaded from a
// byte the body can't change, so that the `Pop` can load it again and
// the `PHA` / `PLA` pair can go. That's the shape
//
//     LDA M; PHA; TXA; TAY; PLA       ->      TXA; TAY; LDA M
//
// which `_translate_indirect_indexed_store` leaves for a `(zp),Y` store
// of `M` at an index that's in X. M is a `Data` or `ZP` operand that
// isn't volatile, and every instruction of the body moves a value into
// a register, and none reads the pushed A before the body writes A
// itself. A load sets N and Z from M as the `PLA` did, and neither
// touches C or V.
std::optional<std::size_t> find_reload_pop(Instrs instrs, std::size_t i) {
  if (i == 0) {
    return std::nullopt;
  }
  const auto* load = as<ir::Mov>(instrs[i - 1]);
  if (!(load != nullptr && is_reg_a(load->dst) && !load->is_volatile &&
        isa<ir::Data, ir::ZP>(load->src))) {
    return std::nullopt;
  }
  bool a_written = false;
  for (std::size_t j = i + 1; j < instrs.size(); ++j) {
    const ir::Type_instruction& instr = instrs[j];
    if (const auto* p = as<ir::Pop>(instr)) {
      if (is_reg_a(p->dst) && a_written) {
        return j;
      }
      return std::nullopt;
    }
    const auto* mov = as<ir::Mov>(instr);
    if (!(mov != nullptr && isa<ir::Reg>(mov->dst))) {
      return std::nullopt;
    }
    if (reads_a(instr) && !a_written) {
      return std::nullopt;
    }
    if (is_reg_a(mov->dst)) {
      a_written = true;
    }
  }
  return std::nullopt;
}

// Finds the pairs to drop in one walk and builds the instructions in a
// second.
void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::set<std::size_t> drop;
  // A Pop to replace with the load it restores (`find_reload_pop`).
  std::map<std::size_t, ir::Type_instruction> reload;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const auto* push = as<ir::Push>(instrs[i]);
    if (push != nullptr && is_reg_a(push->src)) {
      const auto j = find_matching_pop(instrs, i + 1);
      if (j && flags_dead_at(instrs, labels, *j + 1)) {
        drop.insert(i);
        drop.insert(*j);
        // Goes on past the matched Pop, so the body's instructions
        // aren't examined as heads of a pair.
        i = *j + 1;
        continue;
      }
      if (const auto k = find_reload_pop(instrs, i)) {
        // The load moves from in front of the PHA to the PLA.
        drop.insert(i - 1);
        drop.insert(i);
        reload.insert_or_assign(*k, instrs[i - 1]);
        i = *k + 1;
        continue;
      }
    }
    ++i;
  }
  if (drop.empty()) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (drop.count(k) != 0) {
      continue;
    }
    const auto it = reload.find(k);
    out.push_back(it != reload.end() ? it->second : std::move(instrs[k]));
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_dead_pha_pla(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
