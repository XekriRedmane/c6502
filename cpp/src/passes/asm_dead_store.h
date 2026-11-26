#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/asm_liveness.h"

namespace c6502::passes::asm_dead_store {

// Returns whether `instr` is one of the instructions the walk takes to
// read any byte (`Call`, `FunctionPrologue`, `AllocateStack`).
bool is_opaque(const asm_ast::Type_instruction& instr);

// Returns the names that an `ImmLabelLow` / `ImmLabelHigh` operand of
// `instrs` names, and the `Data` source of each `LoadAddress`: the
// names whose address the function takes.
NameSet collect_address_taken_names(Instrs instrs);

// Returns the indices of the instructions that can run right after
// `instrs[i]`, which are none for a `Ret` / `Return`.
std::vector<std::size_t> successors(Instrs instrs, std::size_t i,
                                    const LabelMap& labels);

// Returns whether `target` is dead when the function returns: a ZP byte
// in the pool ($80 to $FF), DPTR, a `__local_*` slot, or a slot that
// `zp_slot_symbols` (which may be null) puts in the pool.
bool is_dead_at_exit(const asm_ast::Type_operand& target,
                     const SymbolAddrs* zp_slot_symbols);

// Returns whether `target` is dead at a tail call, a `Jump` out of the
// function. Only a `__local_*` slot whose address the function doesn't
// take (`address_taken_names`, which may be null) is.
bool is_dead_at_tail_call(const asm_ast::Type_operand& target,
                          const NameSet* address_taken_names);

// Returns whether the value of the byte `target` after `instrs[idx]` is
// read on no path forward, by the walk that decides whether a store is
// dead. `fn_name` is the name of the function the instructions are
// from.
bool mem_dead_after(Instrs instrs, std::size_t idx,
                    const asm_ast::Type_operand& target, const AsmContext& ctx,
                    std::string_view fn_name,
                    const SymbolAddrs* zp_slot_symbols = nullptr);
bool mem_dead_after(Instrs instrs, const LabelMap& labels, std::size_t idx,
                    const asm_ast::Type_operand& target, const AsmContext& ctx,
                    std::string_view fn_name,
                    const SymbolAddrs* zp_slot_symbols = nullptr);

// Returns whether the byte `target` written at `instrs[start]` is read
// on no path forward, by the walk of `mem_dead_after`, with the
// function's `__local_<fn>__` prefix (`local_prefix`) and the names
// whose address it takes (`collect_address_taken_names`) given.
bool is_dead_cfg(Instrs instrs, std::size_t start, const LabelMap& labels,
                 const AsmContext& ctx, const SymbolAddrs* zp_slot_symbols,
                 std::string_view local_prefix,
                 const NameSet& address_taken_names,
                 const asm_ast::Type_operand& target);

// Returns whether a direct `Call` can neither read nor write `target`:
// a `Data` byte whose name starts with `local_prefix` (the function's
// own `__local_<fn>__` slots) and whose address the function never
// takes.
bool call_cannot_touch(const asm_ast::Type_operand& target,
                       std::string_view local_prefix,
                       const NameSet& address_taken_names);

// Returns whether `instr` may read the byte `target`. An access through
// a pointer reads the pointer's bytes too. With `is_private`, `target`
// is a byte no pointer can reach, so an access through a pointer reads
// it only when it is one of the pointer's bytes.
bool reads(const asm_ast::Type_instruction& instr,
           const asm_ast::Type_operand& target, const AsmContext& ctx,
           const SymbolAddrs* zp_slot_symbols = nullptr, bool is_private = false);

// Returns whether `instr` writes exactly the byte `target`: the same
// `Data` name and offset, or the same `ZP` address and offset.
bool writes_same(const asm_ast::Type_instruction& instr,
                 const asm_ast::Type_operand& target);

// Returns the memory operand that `instr` writes, if any: the
// destination of a `Mov`, `Inc`, `Dec`, or `Pop` that isn't a register,
// and the low byte of a `LoadAddress`'s destination.
std::optional<asm_ast::Type_operand> write_operand(
    const asm_ast::Type_instruction& instr);

// Returns every memory operand `instr` may read, including the bytes of
// the pointer that an indirect-Y destination is written through.
std::vector<asm_ast::Type_operand> read_operands(
    const asm_ast::Type_instruction& instr);

// Returns the operands naming the pointer bytes an indirect-Y operand
// goes through: DPTR's two bytes for `Indirect` / `IndirectY`, and the
// two `ZP` bytes at the base of `IndirectZp` / `IndirectZpY`. Nothing
// for any other operand.
std::vector<asm_ast::Type_operand> ptr_source_reads(const asm_ast::Type_operand& op);

}  // namespace c6502::passes::asm_dead_store

namespace c6502::passes {

// Drops each store of a register into a `ZP` or `Data` byte that no
// instruction reachable from it reads, shortens a memory-to-memory move
// whose store is dead to its load, and drops a dead `INC`, `DEC`, or
// shift of a `__local_*` slot whose flags are dead too.
// docs/optimizations/asm_dead_store.md gives the rules.
// `zp_slot_symbols`, which may be null, gives the zero-page address of
// each slot symbol.
asm_ast::Program apply_asm_dead_store(asm_ast::Program prog, const AsmContext& ctx,
                                      const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
