// The port of passes/optimization_asm/liveness.py, the liveness of the
// pseudos of an asm function in SSA form, by block and by instruction.
#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/optimization_asm/cfg.h"

namespace c6502::passes {

// A snapshot of a function's liveness. `live_in[b]` and `live_out[b]`
// are the names live at the start and the end of block `b`, by block
// id. The CFG keeps index ranges into the function's instructions, so
// the function has to outlive the snapshot and stay unchanged while it
// is used, as a change to the function invalidates the Python snapshot
// too.
struct Liveness {
  Cfg cfg;
  std::vector<NameSet> live_in;
  std::vector<NameSet> live_out;

  // Returns the names live right after instruction `i` of block `bid`.
  // The sets of a block are computed the first time one of them is
  // asked for and kept.
  const NameSet& live_after(int bid, std::size_t i) const;
  // Returns the names live right before instruction `i` of block `bid`.
  // Before the first instruction that is the block's `live_in` with its
  // Phi destinations, which are defined at the block's entry.
  NameSet live_before(int bid, std::size_t i) const;

  // The sets `live_after` has computed, by block id. Only `live_after`
  // reads or writes it.
  mutable std::vector<std::optional<std::vector<NameSet>>> per_instr_after;
};

// Returns the liveness of `fn`, from a CFG built afresh and the
// per-block sets solved by iterating the dataflow equations.
Liveness compute_liveness(const asm_ast::Function& fn);

// Returns the liveness of `fn` with the given per-block sets, which the
// checks of the captured calls read from the record.
Liveness liveness_with_sets(const asm_ast::Function& fn, std::vector<NameSet> live_in,
                            std::vector<NameSet> live_out);

namespace liveness {

// Returns the pseudos `instr` writes: the `dst` of a `Mov`, `Pop`,
// `LoadAddress`, `Phi`, `Add`, `Sub`, `And`, `Or`, `Xor`, `Inc`,
// `Dec`, shift, or rotate when it's a pseudo, and the pseudo `dst`
// bytes of a `DecimalArith`.
std::vector<const asm_ast::Pseudo*> defs_in(const asm_ast::Type_instruction& instr);

// Returns the pseudos `instr` reads. A read-modify-write destination
// counts as a read, as the `dst` of `Add`, `Sub`, `And`, and `Or`
// (where `Xor` reads its two sources only) and the operand of `Inc`,
// `Dec`, a shift, or a rotate do. The sources of a `Phi` are read on
// the edges from its predecessors and count as reads of none of its
// own block's instructions.
std::vector<const asm_ast::Pseudo*> uses_in(const asm_ast::Type_instruction& instr);

}  // namespace liveness

}  // namespace c6502::passes
