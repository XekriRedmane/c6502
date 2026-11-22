// The assembler of binutils/assembler.py, which turns dasm text, as
// `asm_emit` writes it, into a relocatable ELF object. `compile.py -c`
// runs it on the text `compile.py -S` would write, and the linker runs
// it again on a unit's `.c6502.asm` text when the program's addresses
// contradict the addressing modes the unit chose (passes/linker.py,
// `_reassemble`). The Python module's docstring describes the subset of
// dasm it reads, the sections and symbols it writes, and how it chooses
// an addressing mode.
//
// The text is read as Python reads a `str`: the lines are split as
// `str.splitlines` splits them, which also breaks at `\x0b`, `\x0c`,
// `\x1c` to `\x1e`, U+0085, U+2028, and U+2029, they are stripped and
// split at Unicode's spaces, a mnemonic is uppercased by `str.upper`,
// a decimal number may be written in any script's digits, and a
// position or a length counts code points. A `DC.B` string is encoded
// as Latin-1, and the object keeps the text as `"\n".join(lines) +
// "\n"`, which turns CR LF into LF.
//
// An expression computes as Python's integers do, with floor division
// and an arithmetic `>>`, but in the 128 bits of `asdl::Int`, and a
// value that takes more gives the OverflowError of src/py_int.h. Python
// stops an expression nested about a hundred parentheses deep, or with
// about a thousand operators in a row, with a RecursionError, so the
// port gives one where its own count of Python's calls passes 400 (see
// `kMaxDepth` in assembler.cpp), and so takes no expression that Python
// doesn't.
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

// The addresses the linker gives the names it places, in the order of
// the Python dict, with none for a name outside zero page.
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
// Throws `CompileError("AssemblerError", ...)` with Python's message for
// a line the assembler can't assemble, the errors of `parse_metadata`
// for a malformed metadata block, a ValueError for a shift by a negative
// count or a decimal number of more than 4,300 digits, as Python's
// integers do, and an OverflowError or a RecursionError where the port
// takes less than Python does.
elf::ElfFile assemble(std::string_view text, std::string_view filename = "<input>",
                      const LinkSymbols* link_symbols = nullptr, bool keep_source = true);

}  // namespace c6502::binutils::assembler
