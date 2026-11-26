#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "ordered_map.h"
#include "passes/asm_context.h"
#include "passes/optimization/pool.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// The soft-stack convention, in which each parameter byte sits in the
// callee's frame at `Frame(M + 3 + j_offset + k)` and the caller pushes
// the arguments.
struct SoftStackLayout {
  bool operator==(const SoftStackLayout&) const = default;
};

// The zero-page convention. `slot_symbols[i]` names the slot of the
// i-th parameter byte, low byte of the first parameter first.
// `addrs[i]` is the address `zp_slot_allocation` gives that slot, or
// none for a slot no instruction uses, and the list is empty before the
// allocator runs. `param_registers[i]` is the register the byte arrives
// in at the call ("A", "X", or "Y"), or none for a byte that arrives in
// its slot. `pinned[i]` is the address `[[c6502::zeropage(addr)]]`
// fixes the byte at, or none. An empty `param_registers` or `pinned`
// means that no byte has one.
struct ZpLayout {
  std::vector<std::string> slot_symbols;
  std::vector<std::optional<asdl::Int>> addrs;
  std::vector<std::optional<std::string>> param_registers;
  std::vector<std::optional<asdl::Int>> pinned;
  bool operator==(const ZpLayout&) const = default;
};

using ParamLayout = std::variant<SoftStackLayout, ZpLayout>;

// Each function's convention, by name, in the order the allocators
// and `tac_to_asm` walk.
using Abi = OrderedMap<std::string, ParamLayout>;

// Returns each function's calling convention for every function in
// `prog`, plus one for each zp_abi extern. An extern that can't have the
// convention gets no entry, and a call to a name without one uses the
// soft stack. An annotated `[[c6502::zp_abi]]` function that can't have
// the convention is an error, `CompileError("AbiSelectionError", ...)`,
// and an unannotated one falls back to the soft stack. `types` is the type
// table, for the sizes of aggregate parameters, and may be null.
// `pool` defaults to the standard caller-saved range, $80-$BF.
Abi select_abi(const tac_ast::Program& prog, const c99_ast::Program& c99_prog,
               const TypeTable* types, const Pool& pool = Pool{});

// Returns the `ZpLayout` of every `zp_abi` function with
// `[[c6502::address(...)]]`, by name in sorted order. A function with an
// address and no `zp_abi` gets no entry, so calls to it use the soft
// stack. Throws `CompileError("AbiSelectionError", ...)` for a
// `register` parameter without `zp_abi`, a parameter byte that isn't in
// a register or at a fixed address, a return that doesn't fit A and X,
// or a taken address.
OrderedMap<std::string, ZpLayout> fixed_address_layouts(const tac_ast::Program& prog,
                                                        const c99_ast::Program& c99_prog,
                                                        const TypeTable* types,
                                                        const Pool& pool = Pool{});

// Returns the slot symbol of every byte of a `zeropage(addr)` parameter
// in `abi`, mapped to its address, for the EQU block.
OrderedMap<std::string, asdl::Int> pinned_slot_symbols(const Abi& abi);

// Returns, per byte of the function's parameters (the `slot_symbols`
// order), the zero-page address a `zeropage(addr)` parameter's byte
// arrives at, or none for the other bytes. It is empty when no
// parameter has one.
std::vector<std::optional<asdl::Int>> pinned_param_addrs(const c99_ast::Type_function_decl& fn_decl,
                                                         const std::vector<asdl::Int>& per_param_bytes);

namespace abi_selection {

// Returns the names of the functions whose address `prog` takes: those
// that appear as a `Var` operand of any instruction, including inside
// the function itself (`fp = f;` in `f`), and those an `AddressInit` or
// `AddressByteInit` of a static's initializer names. `declared` adds the
// functions declared here but defined elsewhere, so an extern whose
// address is taken counts too. A direct call's name isn't an operand,
// and an indirect call's pointer is a variable that a `GetAddress` set,
// which is where the address was taken.
NameSet address_taken(const tac_ast::Program& prog, const NameSet& declared = {});

// Returns one declaration per function name, preferring the one with a
// body, since the types and the parameters agree across them. The
// pointers are into `c99_prog`.
OrderedMap<std::string, const c99_ast::Type_function_decl*> function_decls_by_name(
    const c99_ast::Program& c99_prog);

// Returns whether a `register` local is declared anywhere in the body of
// `fn_decl`, which is false for a declaration without a body.
bool has_register_local(const c99_ast::Type_function_decl& fn_decl);

// Returns whether `fn_decl` gives a 6502 register to any parameter or
// body local (the `register` keyword), or a zero-page address for a
// parameter (`zeropage(addr)`).
bool has_register_request(const c99_ast::Type_function_decl& fn_decl);

}  // namespace abi_selection

}  // namespace c6502::passes
