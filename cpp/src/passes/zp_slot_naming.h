// The parts of passes/zp_slot_naming.py that several code generation
// passes share: the spellings of the zero-page slot symbols and of the
// names they come from.
//
// The compiler makes two families of symbols that bind body locals and
// zp_abi parameters to zero-page addresses. `__zpabi_<fn>__<param>[_<byte>]`
// names a byte of a zp_abi function's parameter, made in
// `abi_selection`, where a one-byte parameter has no `_<byte>`.
// `__local_<fn>__<source>[_<byte>]` names a byte the body-local
// allocator gives a function, where `<source>` is the source spelling
// of the C variable that the slot holds when it traces through SSA
// renaming and move coalescing, and a number when it doesn't.
// `local_slot_names`, which names one function's pool, is in
// src/passes/local_slot_names.h, since the asm optimizer's
// `apply_coloring` reads it too.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "ordered_map.h"
#include "passes/address_taken_zp.h"
#include "passes/zp_local_allocation.h"

namespace c6502::passes {

// Returns the name of the local that a parameter passed in registers is
// copied into at the function's entry, which the body reads in place of
// the parameter: the parameter's name with `.reg` after it. A
// parameter's name has no `.reg` of its own, so this can't collide with
// another variable.
std::string register_param_local(std::string_view param);

// Returns `resolved_name` without the `@<N>.` that
// `identifier_resolution` puts in front of a local's name, the source
// spelling, or the name unchanged when it has no such prefix (a static
// that wasn't renamed).
std::string source_spelling(std::string_view resolved_name);

// Splits an asm-SSA pseudo name into its source name and byte index.
// The asm SSA's `.v<N>` version and `.b<k>` byte suffixes, a TAC SSA
// `.<N>` version, and a register parameter's `.reg` are stripped in
// that order, and what is left is the source name when it is
// `identifier_resolution`'s `@<N>.<orig>`, giving `<orig>`. Any other
// form, such as `%<N>`, gives no source name. The byte index is 0
// without a `.b<k>`. A suffix counts only when what follows `.v` or `.b`
// (or the last `.`) is all ASCII digits, as Python's `str.isdigit`
// gives for the ASCII names the compiler makes.
std::pair<std::optional<std::string>, asdl::Int> parse_pseudo_name(std::string_view name);

// Returns the zp_abi slot symbols of a function's parameters, by
// parameter and then by byte, low byte first, as `abi_selection` numbers
// them: `__zpabi_<fn>__<param>` for a one-byte parameter and
// `__zpabi_<fn>__<param>_<k>` for each byte of a wider one. A name goes
// through `source_spelling`, and an unnamed parameter of a declaration
// (none) is spelled by its position, which no C identifier can spell.
// The linker binds a declaration's symbols to the definition's by
// position, so the spellings needn't match across units. The two lists
// must be the same length, which Python asserts and this throws
// `InternalError` for.
std::vector<std::string> param_slot_symbols(std::string_view fn_name,
                                            const std::vector<std::optional<std::string>>& param_names,
                                            const std::vector<asdl::Int>& param_sizes);

// The slot symbols of every pool byte, by function, each list parallel
// to the function's pool.
using SlotNamesByFunction = OrderedMap<std::string, std::vector<std::string>>;

// Returns the slot symbol of each byte of each function's pool, in the
// pool's order, which compile.py computes once after the last run of the
// asm optimizer and gives to `build_local_slot_symbols` (the `EQU`
// bindings) and `build_metadata` (the linker's records). A function with
// no coloring has numbered names (`__local_<fn>__<k>`), and one with a
// coloring has `local_slot_names`' names. When `assignments` and
// `symbols` (the address-taken locals in zero page and their slot
// symbols) are both given, each pool byte that holds an address-taken
// local takes that local's symbol in place of the name the coloring or
// the numbering gives it. The address-taken pseudo isn't in
// `coloring.assignments`, and the bytes the coloring holds keep their
// names from the source.
SlotNamesByFunction compute_local_slot_names(const LocalPools& local_pools,
                                             const ColoringsByFunction& colorings,
                                             const AddressTakenAssignments* assignments = nullptr,
                                             const AddressTakenSymbols* symbols = nullptr);

// The slot symbols of the pools, as compile.py computes them after the
// asm optimizer's last run: the names of each pool's bytes, and the
// address of each name, which is the `EQU` table of the bodies' locals.
struct LocalSlots {
  SlotNamesByFunction slot_names_by_fn;
  OrderedMap<std::string, asdl::Int> slot_symbols;
};

// Returns the names `compute_local_slot_names` gives the pools' bytes
// and the `EQU` table `build_local_slot_symbols` makes of them, the two
// calls compile.py makes in that order. `assignments` and `symbols` may
// be null, as in `compute_local_slot_names`, which gives what Python's
// `build_local_slot_symbols(local_pools, colorings)` computes when it
// isn't given the names.
LocalSlots local_slots(const LocalPools& local_pools, const ColoringsByFunction& colorings,
                       const AddressTakenAssignments* assignments = nullptr,
                       const AddressTakenSymbols* symbols = nullptr);

}  // namespace c6502::passes
