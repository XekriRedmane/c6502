// The port of passes/optimization_asm/apply_coloring.py, which lowers
// the pseudos of an asm SSA function that the register allocation
// colored to their zero-page bytes or to X and Y.
#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "passes/optimization/register_allocation.h"

namespace c6502::passes {

// Returns `fn` with every pseudo in `coloring.assignments` lowered to its
// byte, `ZP(address, 0)`, and every pseudo in
// `coloring.hwreg_assignments` lowered to `Reg(X)` or `Reg(Y)`.
//
// With `local_pool`, the function's private pool, a colored byte that
// lies in the pool becomes `Data(__local_<fn>_..., 0)` (named by
// `local_slot_names`) in place of the numeric `ZP`. The emitter writes
// an `EQU` for each such name beside the `__zpabi_*` block, and dasm
// picks zero-page or absolute addressing from its value, so a pool that
// spilled above $FF makes no difference to the body. The symbolic form
// lets the linker allocate the body locals of several units again over
// the whole program, since a unit's IR holds slot indices and no
// addresses. A function without a pool keeps the numeric form, and so
// does a colored byte outside its pool, which the allocator, given the
// pool as its `allowed_range`, never produces.
//
// When something is pinned to X or Y, the chains of transfers that the
// substitution leaves before an indexed access are then removed (see
// "Redundant transfers" in apply_coloring.cpp).
asm_ast::Function apply_coloring(asm_ast::Function fn, const Coloring& coloring,
                                 const std::vector<asdl::Int>* local_pool = nullptr);

// The match of a chain of two transfers, the letters of its source and
// destination registers.
struct TransferChain {
  char src;
  char dst;
};

// Matches the chain `Mov(Reg(R'), A); Mov(A, Reg(R))` at
// `instrs[start]`, where R and R' are X or Y. R == R' is a transfer
// through A and back, which does nothing but is still matched, and the
// caller handles it. The unit tests call it, as tests/test_hwreg_pinning.py
// called Python's `_match_transfer_chain`.
std::optional<TransferChain> match_transfer_chain(
    const std::vector<asm_ast::Type_instruction>& instrs, std::size_t start);

// Returns `instrs` without the chains of transfers that the pinning of a
// pseudo to X or Y leaves in the setup of an `IndexedData`'s index
// (Python's `_rewrite_redundant_transfers`, which the unit tests call).
// The comment at its definition gives the rules.
std::vector<asm_ast::Type_instruction> rewrite_redundant_transfers(
    const std::vector<asm_ast::Type_instruction>& instrs);

}  // namespace c6502::passes
