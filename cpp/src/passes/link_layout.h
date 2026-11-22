// The port of passes/link_layout.py, which lays a linked program out:
// where each input section goes, by a linker script (link_script.h) or
// by the default script, which puts `.text`, `.rodata`, `.data`, and
// `.bss` one after another from the origin. The module comment of
// link_layout.cpp describes the layout, the symbols it defines, and the
// fall-through chains.
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "binutils/elf.h"
#include "binutils/ld.h"
#include "ordered_map.h"
#include "passes/link_script.h"
#include "passes/zp_link_metadata.h"

namespace c6502::passes::link_layout {

namespace elf = ::c6502::binutils::elf;

// One input file of the link (Python's `Unit`, which linker.py's
// `_unit` builds). `path` is its name, for messages and for the
// script's file patterns, which match it after its last `/` or `\`.
// `index` is its number in the link, which `Placed::unit` repeats.
// `items` are its functions and objects in the order of its link
// metadata. `sections` gives each item's input section, by the item's
// name, in the items' order, and several items may share a section.
// `tails` gives, for each function that ends in a `JMP` to a function,
// that function's name (`tail_jump`).
struct Unit {
  std::string path;
  int index = 0;
  std::vector<ItemMeta> items;
  OrderedMap<std::string, elf::SectionPtr> sections;
  OrderedMap<std::string, std::string> tails;
};

// An item for `fall_through` (Python's `Entry`): its name, whether it's
// a function, and the function its final `JMP` goes to, if it ends in
// one. `payload` is the caller's, which `fall_through` hands back
// unread. Python's is any object, and `lay_out`'s is the unit and the
// input section, where the C++ caller keeps an index into a list of
// its own.
struct Entry {
  std::string name;
  bool function = false;
  std::optional<std::string> tail;
  std::size_t payload = 0;
};

// The output sections, with every input section placed, and the
// symbols the layout defines, in the order a Python dict gives them:
// each section's bounds and each of the script's assignments (Python's
// `Layout`).
struct Layout {
  std::vector<binutils::ld::OutputSection> outputs;
  OrderedMap<std::string, std::int64_t> symbols;
};

// The value of a symbol that the script names and the layout doesn't
// define, such as an absolute symbol of a unit or a name the linker
// placed, or none (Python's `lookup` argument, which returns None).
using Lookup = std::function<std::optional<std::int64_t>(const std::string&)>;

// Returns the name of the function `sec` jumps to at its end, when the
// function's last instruction is `JMP name` (no offset) and no label or
// data follows it, else none. `symbols` are the symbols of the object
// that holds `sec`, which are compared with `sec` by pointer.
std::optional<std::string> tail_jump(const elf::SectionPtr& sec,
                                     const std::vector<elf::SymbolPtr>& symbols);

// Returns `entries` (in the script's order) with each function that
// ends by jumping to another function of `entries` moved right before
// it, paired with true when its jump is dropped, so the first runs into
// the second. With `reorder` false, only a function already right
// before its target loses its jump, and nothing moves. The comment in
// link_layout.cpp gives the rules.
std::vector<std::pair<Entry, bool>> fall_through(const std::vector<Entry>& entries,
                                                 bool reorder = true);

// Returns the part of a section's symbols named after it: the name
// without its leading dots, with each character other than an ASCII
// letter, digit, or `_` made `_` (`.text.boot` gives `text_boot`).
std::string section_symbol(const std::string& section);

// Returns the script a link without `-T` uses: `.text` from `origin`,
// then `.rodata`, `.data`, `.bss`, and then each other section name
// the units use, in the order they first appear, each right after the
// one before.
link_script::Script default_script(const std::vector<Unit>& units, std::int64_t origin);

// Places every item of `units` by `script`. `lookup` gives the value of
// a symbol the script names that the layout doesn't define, and an
// empty one gives none for every name. Throws
// `CompileError("LayoutError", ...)` with Python's message for an item
// no output section takes, an item that a NOLOAD section can't hold, a
// section with nowhere to go, one that runs past the end of its region,
// and an expression the layout can't evaluate. A value past the 128
// bits the evaluator computes in, or an address past 64 bits, throws
// `InternalError`.
Layout lay_out(const link_script::Script& script, const std::vector<Unit>& units,
               const Lookup& lookup = {});

// Returns the lines of `__zp_init` code that copy each relocated output
// section of `script` to its run address and clear each NOLOAD one, or
// none when there are none. The lines are written with
// `asm_emit::instr_line`, which Python passes in as `instr_line`.
std::vector<std::string> startup_lines(const link_script::Script& script);

}  // namespace c6502::passes::link_layout
