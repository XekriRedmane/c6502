// Decodes the tagged JSON (tools/capture_passes.py's `encode`) of the
// values that the work after the back end's region and emission take,
// for `c6502cpp backend` and for the registry's entries of those
// functions, and writes their results as the capture harness records
// them.
#pragma once

#include <map>
#include <string>
#include <vector>

#include "json.h"
#include "passes/asm_context.h"
#include "passes/prune_unused_slots.h"
#include "passes/sections.h"
#include "passes/zp_globals.h"
#include "passes/zp_link_metadata.h"

namespace c6502::passes::emission_args {

// Returns a tagged dict of names and integers in the dict's order.
OrderedSymbolAddrs ordered_addrs(const json::Value& v);

// Returns what the work reads of a `zp_globals.ZpGlobals` object.
ZpGlobals zp_globals(const json::Value& v);

// Returns the tagged dict of `runtime_layout.helper_footprints()`.
HelperFootprints footprints(const json::Value& v);

// Returns the tagged dict of `sections.section_info`, each name with
// the tuple `(section or None, is_const, mergeable)`.
SectionInfos section_infos(const json::Value& v);

// Returns a `zp_link_metadata.LinkMetadata` object.
LinkMetadata link_metadata(const json::Value& v);

// Returns a tagged dict of function names and sets of symbols, the
// form of `slot_sharing.preferred_symbols`.
std::map<std::string, NameSet, std::less<>> preferred_symbols(const json::Value& v);

// Returns the tagged JSON of `ItemMeta` and `ZpObjectMeta` lists and of
// an ordered dict of addresses, as `json.dumps(encode(result))` writes
// them.
std::string items_json(const std::vector<ItemMeta>& items);
std::string zp_objects_json(const std::vector<ZpObjectMeta>& objects);
std::string ordered_addrs_json(const OrderedSymbolAddrs& addrs);
std::string strings_json(const std::vector<std::string>& strings);

}  // namespace c6502::passes::emission_args
