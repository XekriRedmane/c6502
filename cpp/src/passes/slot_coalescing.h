// The port of passes/slot_coalescing.py.
#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/asm_liveness.h"
#include "passes/slot_sharing.h"

namespace c6502::passes {

// Each function's calling convention as the pass reads it, in the
// order of Python's `abi` dict: a `ZpLayout`, or nothing for a
// soft-stack function.
using AbiLayouts = std::vector<std::pair<std::string, std::optional<ZpLayout>>>;

// The slots each function now writes, by function, in the order of the
// program (the second item of Python's result).
using WrittenSlots = std::vector<std::pair<std::string, std::vector<std::string>>>;

// Returns `prog` with the zero-page locals renamed to the parameter
// slots they copy, as the comment at the top of slot_coalescing.cpp
// describes, and the slots each function now writes. `slot_symbols`
// gives each slot symbol's address.
std::pair<asm_ast::Program, WrittenSlots> apply_slot_coalescing(
    asm_ast::Program prog, const AbiLayouts& abi,
    const std::vector<SlotShare>& shares, const SymbolAddrs& slot_symbols);

// The helpers that ret_slot_coalescing shares.
namespace slot_coalescing {

// Returns the names an instruction of `instrs` refers to in a way the
// pass doesn't follow: at an offset other than 0, by `IndexedData`, in
// a `LoadAddress`, in a volatile access, or in an instruction of a kind
// it doesn't model. Python's `_excluded`.
NameSet excluded(Instrs instrs, const NameSet& names);

// Returns the indices of the instructions that can run right after
// `instrs[k]`, as the pass's liveness sees them: none after a return or
// a jump out of the function (or one with register arguments), the
// target after a jump, and the next instruction and the target after a
// branch. Python's `_successors`.
std::vector<std::size_t> successors(Instrs instrs, std::size_t k,
                                    const LabelMap& label_at);

// Returns, for each instruction that writes one of `names` with the
// value of another, the other's name, by the instruction's index.
// Which register holds which name's value is followed forward through
// the function, and at a label it's what is the same on every path into
// it. Python's `_copy_sources`.
std::map<std::size_t, std::string> copy_sources(Instrs instrs,
                                                const NameSet& names);

}  // namespace slot_coalescing

}  // namespace c6502::passes
