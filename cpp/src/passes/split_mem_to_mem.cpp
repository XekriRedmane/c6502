// Splits `Mov(mem, mem)` into `Mov(mem, Reg(A)); Mov(Reg(A), mem)`.
//
// The asm IR allows a `Mov` whose source and destination are both
// memory operands. The 6502 has no instruction that moves memory to
// memory, so `asm_emit` writes the atom as `LDA src; STA dst`, with A
// as the staging register. One IR atom becomes two instructions.
//
// That compound shape hides the `LDA src` and the write of A from every
// peephole that walks the instruction stream and from the passes that
// track what A holds. `redundant_load_elimination` sees the
// `Mov(M, dst_mem)` atom but leaves its source alone even when its
// tracker has shown that A already holds M, because it only drops bare
// `LDA M` atoms. `round_trip_load` needed its own patterns ("Pattern
// B") to recognize the load inside the move. And the wider class of
// loads made redundant by an earlier computation that left M in A can't
// be found in a memory-to-memory atom at all.
//
// This pass lowers the compound shape into the two atoms it is emitted
// as, so every later peephole and tracker sees the LDA and the STA.
// After the split, `redundant_load_elimination` drops redundant LDAs
// and `asm_dead_store` drops redundant STAs with no special cases.
//
// What gets split
//
// A `Mov(src, dst)` whose source and destination are both memory
// operands in the sense of `asm_emit._is_memory_operand`, which are
// Data, ZP, Frame, Stack, Indirect, IndirectY, IndirectZp, and
// IndirectZpY, and also IndexedData (absolute,X or absolute,Y), which
// `asm_emit` writes as `LDA name,X; STA mem`, the same shape. The split
// form is
//
//     Mov(src, Reg(A))     // LDA src
//     Mov(Reg(A), dst)     // STA dst
//
// and each half takes the move's `is_volatile` bit.
//
// Moves of an operand to itself
//
// `Mov(M, M)` does nothing, and `asm_emit` drops the atom without
// writing a load or a store. This pass drops it too, where a split
// would give a useless `LDA M; STA M`. SSA destruction leaves such a
// move when a Phi's source and destination land on the same byte after
// coalescing.
//
// A register moved to itself, `Mov(X, X)`, is dropped as well. The
// register allocator leaves one when it keeps a register parameter's
// local in the register the parameter arrives in. It emits nothing,
// but a pass that took it for an `LDX` would count it as setting the
// flags. `dead_a_arith` once dropped the `TXA` of `STA p0; Mov(X, X);
// TXA; BMI` as a repeat of it, and the `BMI` then tested `p0`.
// (`asm_liveness.sets_flags` doesn't count one either.)
//
// A move between X and Y
//
// The 6502 has no `TXY` or `TYX`, and `asm_emit` writes `Mov(Reg(X),
// Reg(Y))` as `TXA; TAY`, which changes A too. `kills_a` and the A
// tracker in `redundant_load` don't see that in the one atom. This
// pass splits it into `Mov(Reg(X), Reg(A)); Mov(Reg(A), Reg(Y))`, the
// same instructions, where they do. `counter_to_index_reg` had turned
// `STY i` into `Mov(Reg(Y), Reg(X))` before the loop in Zork's
// `divu16`, and `redundant_load` then dropped the loop's first `LDA`,
// taking A to hold a byte the `TYA` had overwritten.
//
// Volatile moves
//
// A volatile `Mov(src_mem, dst_mem)` stays one atom. The `is_volatile`
// bit is set when either operand is a volatile cell, and the bit
// doesn't say which, so both halves of a split would have to take it.
// The load would then look volatile to `redundant_load_elimination`,
// which would keep it. That pass's handling of a volatile
// memory-to-memory move in `_update_for_mov` adds the source, taken to
// be the operand that isn't volatile, to what A holds, so a later
// explicit `LDA src` is still dropped.
//
// What stays as it is
//
// Every move outside those shapes. `Mov(Imm, mem)` and
// `Mov(ImmLabel*, mem)` are already a load of an immediate and a store
// to `asm_emit`, and `apply_remat` and
// `apply_memory_value_propagation` find their rewrites more easily in
// the original shape. `Mov(Reg, mem)` and `Mov(mem, Reg)` are a single
// store or load, and `Mov(Reg, Reg)` with A at one end is a single
// transfer.
//
// Where it runs
//
// Near the top of `_peephole_fixedpoint`'s sweep. Earlier passes of the
// fixed point, such as `apply_memory_value_propagation`, can make new
// memory-to-memory moves when they put a staging cell's tracked value
// into an operand, and running the split inside the fixed point lowers
// those on the next sweep.
#include "passes/split_mem_to_mem.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// Python: `_is_mem`, with the classes of `_MEM_KINDS`.
bool is_mem(const ir::Type_operand& op) {
  return isa<ir::Data, ir::ZP, ir::Frame, ir::Stack, ir::Indirect,
             ir::IndirectY, ir::IndirectZp, ir::IndirectZpY,
             ir::IndexedData>(op);
}

// What the pass does with one instruction.
enum class Action {
  kKeep,
  kDrop,
  // A move between X and Y, split through A with both halves not
  // volatile.
  kSplitRegisters,
  // A move from memory to memory, split through A with both halves
  // taking the move's `is_volatile` bit.
  kSplitMemory,
};

Action action_for(const ir::Type_instruction& instr) {
  const ir::Mov* mov = as<ir::Mov>(instr);
  if (mov == nullptr) {
    return Action::kKeep;
  }
  const ir::Reg* src_reg = as<ir::Reg>(mov->src);
  const ir::Reg* dst_reg = as<ir::Reg>(mov->dst);
  if (dst_reg != nullptr && mov->src == mov->dst) {
    // `Mov(X, X)`, as the regalloc leaves for a register parameter
    // kept in the register it arrives in. It emits nothing, and a pass
    // that took it for an `LDX` would count it as setting the flags.
    return Action::kDrop;
  }
  if (src_reg != nullptr && dst_reg != nullptr &&
      isa<ir::X, ir::Y>(src_reg->reg) && isa<ir::X, ir::Y>(dst_reg->reg)) {
    // X to Y or Y to X, which is emitted through A.
    return Action::kSplitRegisters;
  }
  if (!mov->is_volatile && is_mem(mov->src) && is_mem(mov->dst)) {
    if (mov->src == mov->dst) {
      // A move of a byte to itself, which `asm_emit` drops too.
      return Action::kDrop;
    }
    return Action::kSplitMemory;
  }
  return Action::kKeep;
}

void split_through_a(const ir::Mov& mov, bool is_volatile,
                     std::vector<ir::Type_instruction>& out) {
  const ir::Reg a{.reg = ir::A{}};
  out.push_back(ir::Mov{.src = mov.src, .dst = a, .is_volatile = is_volatile});
  out.push_back(ir::Mov{.src = a, .dst = mov.dst, .is_volatile = is_volatile});
}

// Rewrites the function's instructions in place. A function with
// nothing to rewrite isn't touched.
void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  const auto first = std::find_if(
      instrs.begin(), instrs.end(),
      [](const ir::Type_instruction& i) { return action_for(i) != Action::kKeep; });
  if (first == instrs.end()) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size() + 8);
  std::move(instrs.begin(), first, std::back_inserter(out));
  for (auto it = first; it != instrs.end(); ++it) {
    switch (action_for(*it)) {
      case Action::kKeep:
        out.push_back(std::move(*it));
        break;
      case Action::kDrop:
        break;
      case Action::kSplitRegisters:
        split_through_a(std::get<ir::Mov>(*it), false, out);
        break;
      case Action::kSplitMemory: {
        const auto& mov = std::get<ir::Mov>(*it);
        split_through_a(mov, mov.is_volatile, out);
        break;
      }
    }
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_split_mem_to_mem(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
