// The disassembler of `objdump.py -d` (binutils/disasm.py), which turns
// an ELF object or executable into dasm text.
//
// The listing assembles with dasm. Each instruction and data line
// carries its address (its offset, in an object) and its bytes in a
// trailing comment, unless they're turned off:
//
//     apply_bobble:
//        SUBROUTINE
//        LDY   __zpabi_apply_bobble__slot  ; 0000  A4 00
//
// Labels are the file's symbols. A `.` label is local, so `SUBROUTINE`
// follows every function's label, as `asm_emit` writes it. In an object,
// an operand that a relocation fills in is written as the relocation's
// symbol and addend (`#<(buf+2)`, `.case@4-1`, `__local_f__x`). In an
// executable, an address operand is written as the symbol at that
// address when there is one, preferring a label of the function the
// instruction is in, then a global symbol, or else as an offset into the
// object it's inside (`src+1`). A section's size symbol (`__text_size`)
// isn't an address and never names one. An immediate is always written
// as a number there, since nothing records which immediates are halves
// of addresses.
//
// Code and data are told apart by section. A section with
// `SHF_EXECINSTR` is code, except between a `$d` mapping symbol and the
// next `$c` (a jump table). A byte that isn't the start of a documented
// instruction is written as `DC.B`.
//
// An absolute instruction whose operand is below $100 is written with
// dasm's `.w` extension (`LDA.w $0080`), which keeps dasm from choosing
// zero-page addressing. In an executable, the listing starts with each
// output section's `SEG` and `ORG`, and with `RORG` when the section
// runs at another address than it's loaded at, so that dasm puts every
// byte where the executable has it.
#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "binutils/elf.h"

namespace c6502::binutils::disasm {

// What the listing shows: `raw` the bytes and `addresses` the address
// of each line.
struct Options {
  bool raw = true;
  bool addresses = true;
};

// Returns the listing of `elf`'s code sections, or of every loaded
// section with `all_sections`, or only of the sections named in `only`
// when it isn't null.
std::vector<std::string> disassemble(const elf::ElfFile& elf, bool all_sections = false,
                                     const std::set<std::string>* only = nullptr,
                                     const Options& options = {});

// Returns Python's `format(v, "0<width>X")`, or with `lower` its `x`
// form. A negative value is a `-` and then the digits, which the width
// counts, so `format(-5, "04X")` is `-005`.
std::string format_hex(std::int64_t v, int width, bool lower = false);

// Returns Python's `s.ljust(width)`, which pads `s` with spaces to
// `width` code points (UTF-8 here).
std::string ljust(std::string_view s, std::size_t width);

}  // namespace c6502::binutils::disasm
