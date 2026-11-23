// The functions of passes/linker.py that work on the objects of a link
// rather than on their link metadata: the runtime library's object, the
// items and units of each object, the renames in its symbols, the
// names it uses as pointers, its assembly again with the program's
// addresses, the executable's symbol table, and the tables several
// units make with the same bytes. Each keeps the Python function's
// inputs and result, and `link_objects` (a later part of the port)
// calls them. The module comment of linker_objects.cpp describes where
// each runs in a link.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "binutils/elf.h"
#include "binutils/ld.h"
#include "ordered_map.h"
#include "passes/asm_context.h"
#include "passes/link_layout.h"
#include "passes/zp_link_metadata.h"

namespace c6502::passes::linker {

namespace elf = ::c6502::binutils::elf;
namespace ld = ::c6502::binutils::ld;

// The renames of one unit's internal names, old name to new, in the
// order of the Python dict `_rename_locals` returns.
using Renames = OrderedMap<std::string, std::string>;

// The runtime library's object in a link, with the name it goes by
// (`runtime.o`), and its link metadata with its items.
struct Library {
  std::pair<std::string, elf::ElfFile> object;
  LinkMetadata meta;
};

// Returns the object of the runtime's helpers that `objects` call and
// neither define nor get from `defsyms`, with its name and its link
// metadata, or none when they call none (passes/runtime_library.py).
// Throws `global_definitions`'s `LinkError`s.
std::optional<Library> runtime_library(const ld::Objects& objects, const ld::Names& defsyms);

// Returns the items of `obj` in order: its metadata's, or, for an
// object without metadata, one for each function and object symbol, in
// the order of its sections. Throws `CompileError("LinkError", ...)`
// when the metadata names an item the object doesn't define, with
// `path` naming the object.
std::vector<ItemMeta> object_items(const LinkMetadata& meta, const elf::ElfFile& obj,
                                   const std::string& path);

// Returns the first function or object symbol named `name` that `obj`
// defines in a section. Python's `next` raises StopIteration when there
// is none, and this throws `InternalError`.
elf::SymbolPtr item_symbol(const elf::ElfFile& obj, const std::string& name);

// Returns `obj` without its `__zp_init` function and the symbols in its
// section, which the linker replaces with one function for the whole
// program, or `obj` itself when it has none. The result shares the
// other sections and symbols with `obj`.
elf::ElfFile without_init(const elf::ElfFile& obj);

// Returns the layout's unit for object number `k`, named `path`, with
// `items`: each item's input section, and the function each function
// that ends in a `JMP` to another jumps to (`link_layout::tail_jump`,
// which Python passes in as `tail_jump`).
link_layout::Unit unit(int k, const std::string& path, const elf::ElfFile& obj,
                       const std::vector<ItemMeta>& items);

// Renames each symbol of `obj` that `renames` names, except a global
// one the object defines.
void apply_renames(elf::ElfFile& obj, const Renames& renames);

// Returns the names some unit uses as an `(name),Y` or `(name,X)`
// pointer, which has to be in zero page.
NameSet pointer_names(const ld::Objects& objects);

// Assembles again, from the text it keeps (`.c6502.asm`), each unit
// that reaches one of the names the linker placed (`placed`) with
// zero-page addressing where the program puts that name above $FF or in
// `outside` (a slot spilled into `__spill`, or a promoted object put
// back in the data segment), and puts the new object in its place in
// `objects`. `renames` are each unit's renames. Throws
// `CompileError("LinkError", ...)` with Python's message when the
// assembler rejects the text.
void reassemble(ld::Objects& objects, const std::vector<Renames>& renames, const ld::Names& placed,
                const NameSet& outside);

// Returns the executable's symbol table: every unit's file symbol, and
// each symbol it defines in a placed section, at its address; the
// absolute symbols, once each; and the names the linker placed or
// defined, as absolute symbols. `addr_of` gives each placed section's
// part, and `zp_objects` the program's zero-page objects, of which only
// the size of each name's first record is read.
std::vector<ld::OutputSymbol> output_symbols(
    const ld::Objects& objects, const std::vector<ld::OutputSection>& outputs,
    const ld::PlacedBySection& addr_of, const ld::Names& placed_names,
    const OrderedMap<std::string, std::vector<ZpObjectMeta>>& zp_objects,
    const ld::Names& layout_symbols, const ld::Names& defsyms);

// Keeps one copy of the items marked `merge` whose sections have the
// same contents, the first in the order of the units and their items,
// and points every other copy's references at it, in `objects` and in
// the items of `metas`.
void merge_identical(ld::Objects& objects, std::vector<LinkMetadata>& metas);

// The symbol a relocation of unit `unit` names, as `contents` compares
// it: by its name alone when it's undefined or global, which every unit
// resolves alike, and by its unit and name when it's local (Python's
// `(k, s.name)`).
struct Target {
  bool local = false;
  int unit = 0;
  std::string name;

  bool operator==(const Target&) const = default;
};

// What the section of an item holds, for `merge_identical` (Python's
// tuple): its name, type, flags, alignment, size, bytes, and its
// relocations as offset, type, target, and addend, sorted.
struct Contents {
  std::string name;
  std::uint32_t type = 0;
  std::uint32_t flags = 0;
  std::int64_t align = 0;
  std::int64_t size = 0;
  std::string data;
  struct Reloc {
    std::int64_t offset = 0;
    std::uint8_t type = 0;
    Target target;
    std::int64_t addend = 0;

    bool operator==(const Reloc&) const = default;
  };
  std::vector<Reloc> relocs;

  bool operator==(const Contents&) const = default;
};

// Returns what the section of item `sym` of unit `k` holds, or none
// when another symbol is defined inside it, which a reference could
// name.
std::optional<Contents> contents(int k, const elf::ElfFile& obj, const elf::SymbolPtr& sym);

}  // namespace c6502::passes::linker
