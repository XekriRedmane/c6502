// The port of passes/sections.py's `items`, the section of each
// function and static object of the finished asm, for the link
// metadata's `item` lines.
#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "asm_ast.h"
#include "passes/zp_link_metadata.h"

namespace c6502::passes {

// What `items` reads of a top-level name's symbol
// (`sections.section_info` computes it in Python): the section that
// `[[c6502::section("name")]]` names, whether the object is `const`,
// and whether it is `StaticAttr.mergeable`.
struct SectionInfo {
  std::optional<std::string> section;
  bool is_const = false;
  bool merge = false;
};

// The `SectionInfo` of each top-level name that has a symbol. A name
// with no entry has no symbol, as `__zp_init` has none.
using SectionInfos = std::map<std::string, SectionInfo, std::less<>>;

// Returns an `ItemMeta` for each function and object of `prog`, in
// order. A function goes in its named section or `.text`. An object goes
// in its named section, or in `.rodata` when it is `const` or a string
// literal (`str@N`, which C99 §6.4.5.6 makes undefined to change), in
// `.bss` when every initializer is a `ZeroInit`, and in `.data`
// otherwise.
std::vector<ItemMeta> section_items(const asm_ast::Program& prog, const SectionInfos& symbols);

}  // namespace c6502::passes
