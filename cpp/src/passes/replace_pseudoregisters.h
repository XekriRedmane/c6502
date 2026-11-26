// Replaces each `Pseudo` operand with a concrete operand and lays out
// each function's frame: `replace_program` for the unoptimized
// pipeline, `replace_program_bare_exit` for `-O` with the frames it
// returns, and `sizeof_type` and `size_of_name`, which most of the code
// generation passes read.
#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "c99_ast.h"
#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/address_taken_zp.h"
#include "passes/asm_context.h"
#include "passes/type_checking.h"
#include "passes/zp_local_allocation.h"

namespace c6502::passes {

// Returns the bytes a value of type `t` occupies: 1 for the character
// types, 2 for `int`, `unsigned int`, and pointers (the 6502's address
// width), 4 for the `long`s and `float`, 8 for the `long long`s and
// `double`, ceil(N/8) for `_BitInt(N)`, the element's size times the
// count for an array, and the layout's size for a struct or union.
// `Const` and `Volatile` are transparent. A struct or union with no
// type table (`types` null), or one whose layout is missing or
// incomplete, counts as 1 byte, and so does any other type.
asdl::Int sizeof_type(const c99_ast::Type_data_type& t, const TypeTable* types);

// Returns the bytes the pseudo `name` occupies, `sizeof_type` of its
// symbol's type, or 1 when `symbols` is null or has no entry for it.
asdl::Int size_of_name(std::string_view name, const SymbolTable* symbols, const TypeTable* types);

// A function's frame, as `replace_program_bare_exit` computes it for
// `prologue_synthesis`: the bytes of its arguments and of its locals
// in the frame, and the callee-saved zero-page bytes it saves, which
// are the payload of `FunctionPrologue` and `Ret`.
struct FrameDims {
  asdl::Int arg_bytes = 0;
  asdl::Int local_bytes = 0;
  std::vector<asdl::Int> callee_saved_addrs;
  bool operator==(const FrameDims&) const = default;
};

// Each function's frame, by name, as `replace_program_bare_exit`
// returns it.
using FrameDimsByFunction = OrderedMap<std::string, FrameDims>;

// Returns `prog` with every function's frame laid out and its pseudos
// replaced, for the unoptimized pipeline. A pseudo naming a static (a
// `StaticVariable` or a `Function` of `prog`, or a name in
// `extra_statics`, such as an `extern` object with no definition here)
// becomes `Data(name, offset)`, a parameter a `Frame` slot above the
// saved FP, a name `colorings[fn]` colors `ZP(address, offset)`, and any
// other name a `Frame` slot of the locals, in the order the walk first
// sees it. Each function starts with a `FunctionPrologue`, and each
// `Ret` carries the frame's dimensions. `symbols`, `types`, and
// `colorings` may be null. A pseudo that is none of those throws
// `InternalError`, and so does a `Phi`.
asm_ast::Program replace_program(asm_ast::Program prog, const NameSet& extra_statics,
                                 const SymbolTable* symbols, const TypeTable* types,
                                 const ColoringsByFunction* colorings = nullptr);

// The arguments of `replace_program_bare_exit` that the unoptimized
// form doesn't take, each of which may be null. `param_layouts` gives
// each function's calling convention, whose `ZpLayout` puts the
// parameters in their slot symbols. `local_pools` gives each function's
// private pool, whose bytes are never saved around the function, and
// `address_taken_assignments` and `address_taken_symbols` the
// address-taken locals placed in it.
struct BareExitTables {
  const Abi* param_layouts = nullptr;
  const LocalPools* local_pools = nullptr;
  const AddressTakenAssignments* address_taken_assignments = nullptr;
  const AddressTakenSymbols* address_taken_symbols = nullptr;
};

// Returns `prog` with its pseudos replaced as `replace_program` does,
// for `-O`, and each function's `FrameDims`. No `FunctionPrologue` is
// added and the bare `Return`s stay, for `synthesize_prologue`. A
// parameter of a zp_abi function becomes `Data(slot_symbol, 0)`, an
// address-taken local placed in the pool `Data(its slot symbol,
// offset)` (or `ZP(address, offset)` without one), and the frame of a
// zp_abi function has no argument bytes.
std::pair<asm_ast::Program, FrameDimsByFunction> replace_program_bare_exit(
    asm_ast::Program prog, const NameSet& extra_statics, const SymbolTable* symbols,
    const TypeTable* types, const ColoringsByFunction* colorings, const BareExitTables& tables);

}  // namespace c6502::passes
