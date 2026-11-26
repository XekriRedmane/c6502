#pragma once

#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "binutils/elf.h"

namespace c6502::binutils::assembler {

// The addresses the linker gives the names it places, with none for a
// name outside zero page.
using LinkSymbols = std::vector<std::pair<std::string, std::optional<asdl::Int>>>;

// Returns whether the linker gives `name` its address: one of the
// runtime's names (SSP, FP, HARGS, and DPTR), a zp_abi parameter slot
// (`__zpabi_*`), a body-local slot (`__local_*`), or one of the unit's
// zero-page objects `zp_objects`.
bool is_link_placed(std::string_view name, const std::set<std::string, std::less<>>& zp_objects);

// Returns the relocatable object for the dasm text `text` (UTF-8).
// `filename` names it in messages and in the object's `STT_FILE`
// symbol. `link_symbols`, when not null, gives the addresses of names
// the linker places, for choosing addressing modes, over the values the
// text's `EQU`s give them, and those names stay undefined in the object.
// With `keep_source`, the object keeps the text in its section
// `.c6502.asm`, so that the linker can assemble the unit again.
//
// Throws `CompileError("AssemblerError", ...)` for a line the assembler
// can't assemble, the errors of `parse_metadata` for a malformed
// metadata block, a ValueError for a shift by a negative count or a
// decimal number of more than 4,300 digits, an OverflowError for a value
// wider than 128 bits, and a RecursionError for an expression nested too
// deeply.
elf::ElfFile assemble(std::string_view text, std::string_view filename = "<input>",
                      const LinkSymbols* link_symbols = nullptr, bool keep_source = true);

}  // namespace c6502::binutils::assembler
