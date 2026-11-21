// The port of passes/optimization_asm/optimizer.py, the driver of the
// asm optimizer, which runs the asm SSA round trip with the register
// allocation on each function of a program.
#pragma once

#include <optional>
#include <set>
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/optimization/pool.h"
#include "passes/optimization/register_allocation.h"
#include "passes/optimization_asm/ssa_construction.h"
#include "passes/type_checking.h"

namespace c6502::passes {

// The keyword arguments of `optimize_program`, with Python's defaults.
// A pointer that is null stands for Python's None.
struct AsmOptimizerOptions {
  // The names of static storage that the program doesn't define with a
  // `StaticVariable` (such as `extern` references), which byte-granular
  // SSA renaming must leave alone so that the asm still names the
  // addresses of the link. The program's own statics and functions are
  // added to them.
  NameSet extra_statics;
  // Each function's calling convention from `select_abi`. For a
  // function with a `ZpLayout`, the layout's addresses are kept out of
  // the coloring, so that the body's locals don't overwrite the bytes
  // of the incoming parameters. Without it every function is taken to
  // have the soft-stack convention and no address is reserved.
  const Abi* param_layouts = nullptr;
  // The symbol table, which `fold_const_statics` reads for the const
  // qualifier of each static, and which gives the `register` objects.
  // Without it the fold does nothing.
  const SymbolTable* symbols = nullptr;
  // Each function's private pool from `allocate_function_locals`, a
  // contiguous list of byte addresses, usually in zero page and possibly
  // spilled above $FF. A function in it takes every color from its pool,
  // and one that isn't falls back to the split of `Pool(start=0x80)`
  // into caller-saved and callee-saved bytes. This is the call-graph
  // disjoint allocation that removes the callee's saves from the
  // prologues of the functions eligible for it.
  const OrderedMap<std::string, std::vector<asdl::Int>>* local_pools = nullptr;
  // The zero-page bytes no function's coloring may take, those of the
  // zero-page globals (passes/zp_globals.py).
  std::set<asdl::Int> reserved_zp;
  // Each function on a cycle that saves around its recursive calls,
  // mapped to its cycle's members (passes/recursion_saves.py). Nothing
  // the allocation can color may be live across a call to one of them,
  // since `tac_to_asm` saved every such value on the hardware stack, and
  // a function that breaks this throws an `InternalError` where it would
  // otherwise miscompile.
  const OrderedMap<std::string, NameSet>* recursion_saves = nullptr;
};

// The program the asm optimizer returns, with the coloring of each
// function by name in the program's order, which
// `replace_pseudoregisters_bare_exit` reads to lower the colored
// pseudos. A static has no coloring.
struct OptimizedAsmProgram {
  asm_ast::Program prog;
  OrderedMap<std::string, Coloring> colorings;
};

// Runs the asm SSA round trip on every function of `prog` and returns
// the program with each function's coloring. `StaticVariable`s are
// passed through, and the statics of internal linkage that nothing names
// afterwards are dropped.
OptimizedAsmProgram optimize_program(asm_ast::Program prog, const AsmOptimizerOptions& options = {});

// The keyword arguments of `optimize_function`, Python's
// `_optimize_function`.
struct AsmFunctionOptions {
  // The `register` locals, each pinned to Y, which is a hard pin.
  const OrderedMap<std::string, std::string>* local_pins = nullptr;
  // The `register` parameters and their entry locals, each a soft hint
  // to the register it arrives in.
  const OrderedMap<std::string, std::string>* param_hints = nullptr;
  // The registers each byte of a register parameter's entry local
  // mustn't take (`entry_register_limits`).
  const OrderedMap<std::string, NameSet>* entry_limits = nullptr;
  // The members of the function's cycle when it saves around recursive
  // calls.
  const NameSet* recursive_callees = nullptr;
  // False for a zp_abi function, whose parameters are read from their
  // slots, and true for a soft-stack one, whose parameters are in its
  // frame.
  bool params_in_frame = true;
  // False when the function's address-taken locals get bytes of its
  // private pool (passes/address_taken_zp.py), and true when they get
  // frame slots.
  bool address_taken_in_frame = true;
};

// One function the asm optimizer returns, with its coloring.
struct OptimizedAsmFunction {
  asm_ast::Function fn;
  Coloring coloring;
};

// Runs the asm SSA round trip on one function: the fold of `SBC #1`
// with a test against zero, explicit tests of moves into X and Y before
// a branch, SSA construction, the fixed point of copy propagation in
// both directions with byte-granular dead code elimination and the
// absorption of zero loads, the register allocation with move coalescing
// and pinning to X and Y, the coloring, and SSA destruction.
// `blocked_addrs` are the bytes the coloring must avoid,
// `allowed_range` the range of the function's private pool, and
// `local_pool` the pool itself, which names the colored bytes.
OptimizedAsmFunction optimize_function(asm_ast::Function fn, const StaticNames& statics,
                                       const std::set<asdl::Int>& blocked_addrs,
                                       const std::optional<Range>& allowed_range = std::nullopt,
                                       const std::vector<asdl::Int>* local_pool = nullptr,
                                       const AsmFunctionOptions& options = {});

namespace optimizer {

// Returns, for the local each `register` parameter of `fn` is copied
// into at entry, the registers each of its bytes mustn't be colored to,
// keyed `<local>.b<k>` (see the comment in optimizer.cpp). The result is
// empty unless `layout` is a `ZpLayout` and `symbols` is given.
OrderedMap<std::string, NameSet> entry_register_limits(const asm_ast::Function& fn,
                                                       const ParamLayout* layout,
                                                       const SymbolTable* symbols);

// Returns `fn` with a `CPX #0` (or `CPY #0`) before each branch on N or
// Z that reads the flags of a move into X (or Y) just before it, or
// `INX; DEX` when the code the branch reaches reads C.
asm_ast::Function explicit_index_tests(asm_ast::Function fn);

}  // namespace optimizer

}  // namespace c6502::passes
