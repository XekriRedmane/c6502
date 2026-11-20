// The port of passes/optimization_asm/ssa_construction.py, the asm SSA
// form with each byte of a Pseudo versioned on its own.
#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>

#include "asm_ast.h"

namespace c6502::passes {

// The static-storage names a pass of the asm SSA round trip is given,
// as the optimizer's `statics` frozenset.
using StaticNames = std::set<std::string, std::less<>>;

// The Pseudo names a pass of the round trip leaves unversioned.
using ExcludedNames = std::unordered_set<std::string>;

// A byte of a Pseudo, its name and its byte offset.
using ByteVar = std::pair<std::string, std::int64_t>;

// Returns `fn` in asm SSA form, with a `Phi` at the iterated dominance
// frontier of the definitions of each promotable byte where the byte
// is live, and each definition of byte k of `name` renamed
// `<name>.b<k>.v<N>`. A name in `statics` is never versioned. The
// comment at the top of ssa_construction.cpp gives the rules.
asm_ast::Function to_ssa(const asm_ast::Function& fn, const StaticNames& statics);

// Returns the Pseudo names that keep their spelling and their bytes
// together, which SSA construction, both copy propagations, and byte
// DCE leave unversioned: names whose address is taken or that a
// `LoadAddress` writes, the targets of a read-modify-write, the
// pointers staged through DPTR, and the names a volatile `Mov` reads
// or writes.
ExcludedNames excluded_pseudo_names(const asm_ast::Function& fn);

// Returns whether `name` is in `excluded` or in `statics`.
inline bool is_excluded(const std::string& name, const ExcludedNames& excluded,
                        const StaticNames& statics) {
  return excluded.contains(name) || statics.contains(name);
}

}  // namespace c6502::passes
