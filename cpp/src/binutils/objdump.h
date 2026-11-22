// objdump.py, which shows what's in c6502 objects and executables as
// GNU objdump does, with the disassembly in dasm's syntax (step 7,
// docs/cpp_port.md).
//
//     -d, --disassemble      disassemble the code sections
//     -D, --disassemble-all  disassemble every loaded section, data as DC.B
//     -j, --section NAME     only the sections named NAME (repeatable)
//     -f, --file-headers     the file's type, machine, flags, and entry
//     -h, --section-headers  the sections, with sizes and addresses
//     -t, --syms             the symbol table
//     -r, --reloc            the relocations
//     -s, --full-contents    every loaded section's bytes in hex
//     -x, --all-headers      -f, -h, -t, -r, and the link metadata
//         --link-metadata    the c6502 link metadata
//         --no-show-raw-insn leave the bytes out of the disassembly
//         --no-addresses     leave the addresses out of the disassembly
//
// The disassembly of an executable (`-D`) assembles with dasm into the
// same bytes at the same addresses (src/binutils/disasm.h). The
// disassembly of an object shows each relocation's symbol in the
// operand it fills in, as `objdump -dr` does.
#pragma once

#include <set>
#include <string>
#include <vector>

#include "binutils/elf.h"
#include "binutils/nm.h"

namespace c6502::binutils::objdump {

// Return the lines of `-f`, `-h`, and `-t` for `elf`, with `path` as
// the command line gave it.
std::vector<std::string> file_header(const elf::ElfFile& elf, const std::string& path);
std::vector<std::string> section_headers(const elf::ElfFile& elf);
std::vector<std::string> symbol_table(const elf::ElfFile& elf);

// Return the lines of `-r` and `-s` for `elf`, of the sections named in
// `only` when it isn't null.
std::vector<std::string> relocations(const elf::ElfFile& elf, const std::set<std::string>* only);
std::vector<std::string> full_contents(const elf::ElfFile& elf, const std::set<std::string>* only);

// Runs objdump.py's `main` on the command line `argv` (the program's
// name first), with the files `read_file` gives and argparse's usage and
// help wrapped for `columns` columns, as nm's `main` does (nm.h), and
// returns the exit status. A command line that asks for nothing to be
// shown returns 2 with objdump.py's own message, which Python returns
// where argparse raises SystemExit.
int main(const std::vector<std::string>& argv, const nm::ReadFile& read_file, int columns,
         std::string& out, std::string& err, bool* help = nullptr);

}  // namespace c6502::binutils::objdump
