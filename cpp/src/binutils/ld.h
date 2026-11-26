#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "binutils/elf.h"
#include "ordered_map.h"

namespace c6502::binutils::ld {

// The objects of a link, each with the name it goes by, in the order of
// the link. An `ElfFile` holds its sections and symbols by shared
// pointers, so a copy of one names the same sections and symbols.
using Objects = std::vector<std::pair<std::string, elf::ElfFile>>;

using Names = OrderedMap<std::string, std::int64_t>;

// Input section `section` of object number `unit`, running at `addr`.
// `size` is its size in the output, which is 3 bytes smaller than the
// section's when the linker drops a final `JMP` to the function after
// it. `section` is the object's own section, by pointer, since the
// linker keys the placed sections by identity.
struct Placed {
  int unit = 0;
  elf::SectionPtr section;
  std::int64_t addr = 0;
  std::int64_t size = 0;
};

using PlacedBySection = std::unordered_map<const elf::Section*, Placed>;

// An output section of the executable: its name, its run address
// `addr`, its load address `load_addr` (the same unless the script
// gives `AT>` or `AT(...)`), its size in bytes, the placed input
// sections in address order, whether it has no bytes in the file
// (`NOLOAD`), and whether it holds code. `executable()` sets
// `elf_section` to the section it writes for it, and the layout leaves
// it null.
struct OutputSection {
  std::string name;
  std::int64_t addr = 0;
  std::int64_t load_addr = 0;
  std::int64_t size = 0;
  std::vector<Placed> parts;
  bool noload = false;
  bool executable = false;
  elf::SectionPtr elf_section;
};

// An entry of the executable's symbol table before `executable()`
// writes it. `output` is the index of the output section the symbol is
// defined in, in the list `executable()` takes, or -1 when `symbol->section` and
// `symbol->shndx` stand as they are.
struct OutputSymbol {
  elf::SymbolPtr symbol;
  int output = -1;
};

// The global symbols the objects define in sections, as the number of
// the object and the symbol, and the absolute ones, as values.
struct Definitions {
  OrderedMap<std::string, std::pair<int, elf::SymbolPtr>> defs;
  OrderedMap<std::string, std::int64_t> absolute;
};

// Returns the global symbols the objects define in sections and the
// absolute ones. Throws `CompileError("LinkError", ...)` for a name two
// objects define, an absolute name given two values, and a name that
// is an address in one object and defined in a section in another.
Definitions global_definitions(const Objects& objects);

// Gives each symbol an address, once every section is placed. It keeps
// a pointer to `objects`, which must outlive it.
class Resolver {
 public:
  Resolver(const Objects& objects, const std::vector<Placed>& placed, Names extra);

  // Returns the run address of `sec`, or none when the link left it
  // out.
  std::optional<std::int64_t> section_addr(const elf::Section* sec) const;

  // Returns the address of `sym`, which object number `unit` names: a
  // symbol defined in the object at its section's address plus its
  // offset, else the global symbol another object defines, else an
  // absolute one, else a name the linker defines (`extra`). Throws
  // `CompileError("LinkError", ...)` for a symbol in a section the link
  // left out and for an undefined name.
  std::int64_t value(const elf::Symbol& sym, int unit) const;

  const Definitions& definitions() const { return definitions_; }

 private:
  const Objects* objects_;
  std::unordered_map<const elf::Section*, std::int64_t> addr_;
  Definitions definitions_;
  Names extra_;
};

// Returns the address of a name the linker knows before the layout, or
// none.
using ValueOf = std::function<std::optional<std::int64_t>(const elf::SymbolPtr&)>;

// Shortens each absolute instruction in code section `sec` whose
// operand the linker has put in zero page to its zero-page form, and
// returns the number of bytes saved. `symbols` are the object's
// symbols, whose labels and mapping symbols in `sec` move with the
// bytes. The comment in ld.cpp gives the rules.
std::int64_t relax(const elf::SectionPtr& sec, const std::vector<elf::SymbolPtr>& symbols,
                   const ValueOf& value_of);

// Returns the bytes of placed section `p` with its relocations applied.
// `path` names the object in messages. Throws
// `CompileError("LinkError", ...)` when a value
// doesn't fit its field, for a branch out of reach, and for a type it
// doesn't know.
std::string relocate(const Placed& p, const Resolver& resolver, const std::string& path);

// Returns the executable for `outputs`, with every input section's
// relocations applied, `symbols` as its symbol table, `entry` as its
// entry point, and the merged link metadata `link_text` in its section
// `.c6502.link` when there is one. Sets each output section's
// `elf_section` to the section it writes, and each symbol's section to
// that of its output section.
elf::ElfFile executable(const Objects& objects, std::vector<OutputSection>& outputs,
                        const Resolver& resolver, const std::vector<OutputSymbol>& symbols,
                        std::int64_t entry, const std::optional<std::string>& link_text);

}  // namespace c6502::binutils::ld
