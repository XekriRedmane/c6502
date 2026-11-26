#pragma once

#include <set>
#include <string_view>
#include <vector>

#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

// The default range of the asm-level register allocator's pool,
// `[kDefaultPoolLo, kDefaultPoolHi)`.
inline constexpr int kDefaultPoolLo = 0x80;
inline constexpr int kDefaultPoolHi = 0x100;

struct MayAliasOptions {
  asdl::Int pool_lo = kDefaultPoolLo;
  asdl::Int pool_hi = kDefaultPoolHi;
  // The slot-name to zero-page-address map (`__local_foo__bar` to
  // $8A), or null. With it, `Data("__local_..._1")` at $8A aliases
  // `ZP($8A)` exactly. Without it, any `Data` that names a slot may
  // alias any `ZP` byte.
  const SymbolAddrs* zp_slot_symbols = nullptr;
};

// Returns whether the two operands may name the same byte, which is
// true unless the rules in the comment at the top of asm_aliasing.cpp
// prove them apart. `ctx` gives the user zero-page globals, the
// runtime's layout, and the pointer proof installed by the pass.
bool may_alias(const asm_ast::Type_operand& a, const asm_ast::Type_operand& b,
               const AsmContext& ctx, const MayAliasOptions& options = {});

// Returns the `Data` names of `prog` that no pointer can point to,
// since nothing in the unit takes their address (no `LoadAddress` of
// them, no `#<name` or `#>name`, and no static initialized with their
// address). A `static` object qualifies, and so does a zero-page slot
// `__local_F__*` or `__zpabi_F__*` of a function `F` defined in the
// unit, unless another function's name could own it too.
NameSet unreachable_by_pointer(const asm_ast::Program& prog);

// The names a function's instructions call, take the address of, and
// name in `Data` operands, which `unreachable_by_pointer` reads.
struct FunctionNames {
  NameSet called;
  NameSet taken;
  NameSet seen;
};
FunctionNames function_names(Instrs instrs);

// Returns every operand of `ins`, in field order, including those in a
// list (a `DecimalArith`'s) and the sources of a `Phi`'s arguments.
std::vector<asm_ast::Type_operand> instruction_operands(
    const asm_ast::Type_instruction& ins);

// Returns whether `ins` reads or writes through a `(zp),Y` pointer
// (`IndirectZp`, `IndirectZpY`) with a byte at one of `slot_addrs`, the
// addresses of the unit's `__zpabi_*` and `__local_*` slots. Such an
// instruction can't be moved into another function.
bool through_slot_pointer(const asm_ast::Type_instruction& ins,
                          const std::set<asdl::Int>& slot_addrs);

// Returns the addresses of both bytes of every `(zp),Y` pointer that an
// instruction of `instrs` reads or writes through.
std::set<asdl::Int> pointer_byte_addresses(Instrs instrs);

// Returns whether the `Data` or `ZP` byte `op` is one of
// `pointer_addrs` (from `pointer_byte_addresses`). A `Data` operand's
// symbol is resolved through `zp_symbol_addrs`, which may be null, and
// a compiler slot or runtime byte it doesn't resolve counts as a
// pointer byte whenever the function has a pointer.
bool is_pointer_byte(const asm_ast::Type_operand& op,
                     const std::set<asdl::Int>& pointer_addrs,
                     const SymbolAddrs* zp_symbol_addrs);

// Returns whether `name` is a symbol that resolves to a zero-page byte
// through an EQU line: a runtime symbol (SSP, FP, HARGS, DPTR), a
// zp_abi parameter slot (`__zpabi_*`), or a body-local slot
// (`__local_*`).
bool is_zp_symbol(std::string_view name);

// Returns the address of each runtime zero-page symbol in the layout of
// the compile, by name.
SymbolAddrs runtime_zp_addrs(const AsmContext& ctx);

}  // namespace c6502::passes
