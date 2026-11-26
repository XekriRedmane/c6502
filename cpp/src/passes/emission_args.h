// Decodes the tagged JSON of the values that the work after the back
// end's region and emission take, for `c6502cpp backend` and for the
// registry's entries of those functions, and writes their results in
// the tagged JSON of the frozen capture records.
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

ZpGlobals zp_globals(const json::Value& v);

HelperFootprints footprints(const json::Value& v);

// Returns a tagged dict of names, each with the tuple `(section or
// None, is_const, mergeable)`.
SectionInfos section_infos(const json::Value& v);

LinkMetadata link_metadata(const json::Value& v);

std::map<std::string, NameSet, std::less<>> preferred_symbols(const json::Value& v);

// Returns the tagged JSON of `ItemMeta` and `ZpObjectMeta` lists and of
// an ordered dict of addresses, and the JSON of a list of strings,
// spaced as Python's `json.dumps` spaces it.
std::string items_json(const std::vector<ItemMeta>& items);
std::string zp_objects_json(const std::vector<ZpObjectMeta>& objects);
std::string ordered_addrs_json(const OrderedSymbolAddrs& addrs);
std::string strings_json(const std::vector<std::string>& strings);

}  // namespace c6502::passes::emission_args
