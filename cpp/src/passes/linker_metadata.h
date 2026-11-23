// The port of the functions of passes/linker.py that work on the link
// metadata: the merge of every unit's metadata and its checks, the
// synthetic program the allocators run on, the renames of internal
// names, the placement of the zero-page objects and of the runtime's
// block, the shares across units, the binding of each declaration's
// parameter slots, and the text of the linker's own code. The port of
// `link_objects` (group D3 of step 7) calls them in the order the
// comment at the top of linker_metadata.cpp gives, with the objects and
// the names it builds.
//
// Each function takes and returns what the Python function does. Where
// Python changes an argument in place, the C++ function takes it by
// reference and changes it the same way. Every `LinkError` is a
// `CompileError("LinkError", ...)` with Python's message.
#pragma once

#include <cstddef>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "binutils/elf.h"
#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/slot_sharing.h"
#include "passes/zp_link_metadata.h"
#include "tac_ast.h"

namespace c6502::passes::linker {

namespace elf = ::c6502::binutils::elf;

// The objects of a link, each with the name it goes by, in the order of
// the link (Python's `list[tuple[str, E.ElfFile]]`). A function that
// renames symbols changes the objects' own symbols, which every
// relocation names by pointer, as Python's do by identity.
using Objects = std::vector<std::pair<std::string, elf::ElfFile>>;

// A unit's renames, from each old name to its new one, in the order
// `rename_locals` made them.
using Renames = OrderedMap<std::string, std::string>;

// Every unit's records of each zero-page object, by name, in the order
// the units list the objects (the result of `merge_zp_objects`).
using ZpObjects = OrderedMap<std::string, std::vector<ZpObjectMeta>>;

// The program's runtime layout as Python's `_choose_layout` returns it:
// "SSP", "FP", "HARGS", and "DPTR" with their addresses, then
// "HARGS size", in that order.
using LayoutDict = OrderedMap<std::string, asdl::Int>;

// Names and their addresses, in the order a Python dict holds them.
using SymbolAddrs = OrderedMap<std::string, asdl::Int>;

// Where each parameter byte of a `def` or `ext` record arrives: its
// register ("" for none) and its zero-page address (none for none).
using Arrivals = std::vector<std::pair<std::string, std::optional<asdl::Int>>>;

// Returns every unit's metadata as one (Python's `_merge`). The defs and
// the externs are sorted by name, the call edges and the saved edges
// are the units' edges once each, sorted, and the shares are every
// unit's in order. An extern that several units declare is the first
// unit's record. The records are copies, where Python's merged record
// is the unit's own object, and `link_objects` changes only the merged
// records after this (`drop_unused_locals`). Throws LinkError for a
// declaration whose parameters arrive differently from another's or
// from the definition's, for units that disagree on a carry return, for
// a function two units define, and for an extern whose parameter bytes
// differ between units, in that order.
LinkMetadata merge(const std::vector<LinkMetadata>& metas);

// Returns the message for units that disagree on whether `name` returns
// its `bool` in the carry flag (Python's `_carry_conflict`).
std::string carry_conflict(std::string_view name);

// Returns where each parameter byte of a `def` or `ext` record arrives
// (Python's `_arrivals`).
Arrivals arrivals(const FunctionMeta& meta);
Arrivals arrivals(const ExternMeta& meta);
// Returns the arrivals of a record with `param_bytes` parameter bytes
// and these registers and pinned addresses, which the two above read.
Arrivals arrivals(std::size_t param_bytes, const std::vector<std::string>& param_regs,
                  const std::vector<std::optional<asdl::Int>>& pinned);

// Throws LinkError unless every function `merged` defines can have a
// private pool (Python's `_validate_eligibility`): one with an
// `IndirectCall`, one on a cycle in its own unit that isn't a cycle of
// calls that all save on the hardware stack, and one that calls a name
// no unit defines or declares zp_abi.
void validate_eligibility(const LinkMetadata& merged);

// The program, the ABI, and the local bytes that `synthesize` builds.
struct Synthesized {
  tac_ast::Program prog;
  Abi abi;
  OrderedMap<std::string, asdl::Int> local_bytes;
};

// Returns the program the allocators run on for `merged` (Python's
// `_synthesize`): a function for each def, with a call of each callee
// its call edges give, the ABI of each def and of each extern no unit
// defines, from the slot symbols of the metadata, and each def's number
// of local bytes.
Synthesized synthesize(const LinkMetadata& merged);

// Gives each internal item and internal zero-page object whose name
// another unit also uses, or that `taken` holds, the name `name.k` in
// unit `k`, in the unit's metadata and its object's symbols, with the
// slots of a renamed function, and returns each unit's renames
// (Python's `_rename_locals`). `objects` and `metas` are in the order of
// the link, one metadata for each object. Throws LinkError when two
// units define one external name.
std::vector<Renames> rename_locals(Objects& objects, std::vector<LinkMetadata>& metas,
                                   const NameSet& taken);

// Applies `renames` to every function, slot, item, and zero-page object
// name in `meta` (Python's `_rename_meta`).
void rename_meta(LinkMetadata& meta, const Renames& renames);

// Returns every unit's records of each zero-page object (Python's
// `_merge_zp_objects`). Throws LinkError when the units disagree on an
// object's size or address, two define it, or none defines one that has
// no address.
ZpObjects merge_zp_objects(const std::vector<LinkMetadata>& metas);

// Returns the address of each zero-page object, in the order they were
// placed, and the promoted objects that didn't fit, which go back in the
// data segment (Python's `_place_zp_objects`). The comment at the
// function gives the rules. Throws LinkError on an overlap or a full
// range.
std::pair<SymbolAddrs, std::vector<std::string>> place_zp_objects(
    const ZpObjects& objects, const LayoutDict& layout,
    const std::set<asdl::Int>& param_bytes = {}, const NameSet& pointers = {});

// Returns the program's runtime layout, from every unit's HARGS need
// and the bytes of the zero-page objects with addresses and of
// `param_bytes` (Python's `_choose_layout`). A unit without a need takes
// the classic block's 32 bytes, and the need is at least
// `startup_need`. Throws LinkError when the block doesn't fit.
LayoutDict choose_layout(const std::vector<LinkMetadata>& metas, const ZpObjects& objects,
                         asdl::Int startup_need, const std::set<asdl::Int>& param_bytes = {});

// Returns the bytes of SSP, FP, DPTR, and HARGS in `layout` (Python's
// `_runtime_bytes`).
std::set<asdl::Int> runtime_bytes(const LayoutDict& layout);

// Returns every unit's shares joined into the program's, with each
// declaration's spelling of a slot replaced by the definition's, and
// checked against the program's call graph (Python's `_link_shares`).
// Throws LinkError with the message of `merge_shares` or
// `check_linked_shares`.
std::vector<SlotShare> link_shares(const LinkMetadata& merged,
                                   const std::vector<LinkMetadata>& metas);

// Adds to `symbols` the address of each parameter slot symbol of each
// of `externs` (every unit's declarations, in order), that of the
// definition's slot in the same position (Python's
// `_bind_extern_params`). A function that no unit defines takes the
// first declaration's symbols. Throws LinkError for a declaration with
// a different number of parameter bytes or different registers, and
// for a symbol that would name two bytes. A canonical slot that
// `symbols` lacks, which is Python's KeyError, throws `InternalError`.
void bind_extern_params(const LinkMetadata& merged, const std::vector<ExternMeta>& externs,
                        SymbolAddrs& symbols);

// Leaves out of each def's `locals` in `merged` the slots that no
// relocation of `objects` names, unless a relocation takes the address
// of one of them (Python's `_drop_unused_locals`).
void drop_unused_locals(LinkMetadata& merged, const Objects& objects);

// Returns the lines of the program's `__zp_init` (Python's
// `_init_lines`): `startup` (link_layout's `startup_lines`), then the
// stores of every object's initial bytes, each value loaded once, zero
// first. Empty when there's nothing to store and no startup code.
std::vector<std::string> init_lines(const std::vector<ZpObjectMeta>& objects,
                                    const std::vector<std::string>& startup = {});

// Returns the lines of a promoted object put back in the data segment,
// with its initial bytes (Python's `_data_lines`).
std::vector<std::string> data_lines(const ZpObjectMeta& record);

// Returns `param_regs` padded with "" to `count` entries (Python's
// `_regs`).
std::vector<std::string> regs(const std::vector<std::string>& param_regs, std::size_t count);

}  // namespace c6502::passes::linker
