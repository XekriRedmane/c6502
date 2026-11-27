// The runtime library: the 6502 helpers that compiled code calls to
// multiply, divide, and shift by a variable count, which the linker
// adds to a program as a C toolchain links routines from libgcc. The
// helpers are the functions of src/rtlib/helpers.h.
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "binutils/elf.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// The name the library's object goes by in a link, which a linker
// script's file patterns match.
inline constexpr const char* kLibraryName = "runtime.o";

// Returns the names of the helpers the library has.
NameSet helper_names();

// Returns `names` and every helper they call or jump to, directly or
// through another, in the library's order. Names that aren't helpers
// are left out.
std::vector<std::string> closure(const std::vector<std::string>& names);

// Returns the helpers that a link of `objects` takes from the library:
// the ones some object names in an undefined symbol and no name in
// `defined` (the names the objects define, and those `--defsym` gives)
// covers, and the helpers they call that `defined` doesn't cover
// either, in the library's order.
std::vector<std::string> needed(
    const std::vector<std::pair<std::string, binutils::elf::ElfFile>>& objects,
    const NameSet& defined);

// Returns the text that the library's object for the helpers `names`,
// which `needed` returned, is assembled from: a link metadata block
// that lists each helper as an item in `.text` and gives the number of
// HARGS bytes the helpers touch as the object's HARGS size and need,
// and then the helpers as `asm_emit` writes them.
std::string library_text(const std::vector<std::string>& names);

}  // namespace c6502::passes
