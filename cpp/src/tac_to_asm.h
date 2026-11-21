// The port of tac_to_asm.py, which lowers a TAC program to the asm IR
// with one asm instruction per 6502 opcode. src/tac_to_asm.cpp opens
// with the module's description of the lowerings.
#pragma once

#include <string>

#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/recursion_saves.h"
#include "passes/slot_sharing.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::tac_to_asm {

// The `zeropage(addr)` parameters each function copies into a local as
// it starts (`slot_sharing.pinned_param_copies`).
using PinnedCopies = OrderedMap<std::string, passes::NameSet>;

// Each slot symbol in a share mapped to one spelling for its byte
// (`slot_sharing.slot_aliases`).
using SlotAliases = OrderedMap<std::string, std::string>;

// The keyword arguments of Python's `translate_program` after the
// tables. A null pointer stands for Python's None, which the lowering
// treats as an empty table.
struct Options {
  // Ends each function's returns with a bare `Return`, for the
  // optimized pipeline, where prologue synthesis writes the frame
  // teardown after register allocation. Without it a return is the
  // compound `Ret` that `replace_pseudoregisters` fills in.
  bool bare_exit = false;
  // Each function's calling convention (`abi_selection.select_abi`).
  const passes::Abi* abi = nullptr;
  // Each function on a cycle that gets private pools, mapped to its
  // cycle (`recursion_saves.hw_stack_cycles`, filtered to the eligible
  // ones). Calls between them save the caller's live values on the
  // hardware stack.
  const passes::Cycles* recursion_saves = nullptr;
  // Each function's variables kept in a callee's parameter slots, with
  // the slots' symbols, low byte first (`slot_sharing.find_shares`).
  const passes::Bindings* bindings = nullptr;
  const PinnedCopies* pinned_copies = nullptr;
  const SlotAliases* slot_aliases = nullptr;
};

// Returns the asm program of `prog`. `symbols` is the symbol table
// `c99_to_tac` produced, which gives each variable's size and
// signedness, and to which the lowering adds the `<param>.reg` local of
// each parameter that arrives in a register or is copied from its fixed
// address, in the order Python adds them. It may be null, and then
// every variable is one byte, as in the Python unit tests of synthetic
// programs. `types` is the type table of struct and union layouts and
// may be null. `ctx` gives the user zero-page globals, which decide
// whether a pointer is in zero page (`pointer_restage`).
asm_ast::Program translate_program(const tac_ast::Program& prog, passes::SymbolTable* symbols,
                                   const passes::TypeTable* types, const passes::AsmContext& ctx,
                                   const Options& options = {});

}  // namespace c6502::tac_to_asm
